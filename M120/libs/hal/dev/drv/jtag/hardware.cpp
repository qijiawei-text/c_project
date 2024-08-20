/*********************************************************************************
* Lattice Semiconductor Corp. Copyright 2000-2008
*
* This is the hardware.c of ispVME V12.1 for JTAG programmable devices.
* All the functions requiring customization are organized into this file for
* the convinience of porting.
*********************************************************************************/
/*********************************************************************************
* Revision History:
*
* 09/11/07 NN Type cast mismatch variables
* 09/24/07 NN Added calibration function.
*             Calibration will help to determine the system clock frequency
*             and the count value for one micro-second delay of the target
*             specific hardware.
*             Modified the ispVMDelay function
*             Removed Delay Percent support
*             Moved the sclock() function from ivm_core.c to hardware.c
*********************************************************************************/

#include <unistd.h>
#include "Jtag.h"
#include "hardware.h"

#ifdef __cplusplus
extern "C" {
#endif

extern CJtag *g_pJtag;

/********************************************************************************
 * writePort
 * To apply the specified value to the pins indicated. This routine will
 * be modified for specific systems.
 * As an example, this code uses the IBM-PC standard Parallel port, along with the
 * schematic shown in Lattice documentation, to apply the signals to the
 * JTAG pins.
 *
 * PC Parallel port pin    Signal name           Port bit address
 *        2                g_ucPinTDI             1
 *        3                g_ucPinTCK             2
 *        4                g_ucPinTMS             4
 *        5                g_ucPinENABLE          8
 *        6                g_ucPinTRST            16
 *        10               g_ucPinTDO             64
 *
 *  Parameters:
 *   - a_ucPins, which is actually a set of bit flags (defined above)
 *     that correspond to the bits of the data port. Each of the I/O port
 *     bits that drives an isp programming pin is assigned a flag
 *     (through a #define) corresponding to the signal it drives. To
 *     change the value of more than one pin at once, the flags are added
 *     together, much like file access flags are.
 *
 *     The bit flags are only set if the pin is to be changed. Bits that
 *     do not have their flags set do not have their levels changed. The
 *     state of the port is always manintained in the static global
 *     variable g_siIspPins, so that each pin can be addressed individually
 *     without disturbing the others.
 *
 *   - a_ucValue, which is either HIGH (0x01 ) or LOW (0x00 ). Only these two
 *     values are valid. Any non-zero number sets the pin(s) high.
 *
 *********************************************************************************/

void
writePort(unsigned char a_ucPins, unsigned char a_ucValue)
{
    /* CJtag::WritePin(a_ucPins, a_ucValue); */
    g_pJtag->WritePin(a_ucPins, a_ucValue);
}

/*********************************************************************************
 *
 * readPort
 *
 * Returns the value of the TDO from the device.
 *
 **********************************************************************************/
unsigned char
readPort()
{
    unsigned char ucRet = 0;


    /* CJtag::ReadTDO(ucRet); */
    g_pJtag->ReadTDO(ucRet);

    return ucRet;
}

/*********************************************************************************
* sclock
*
* Apply a pulse to TCK.
*
* This function is located here so that users can modify to slow down TCK if
* it is too fast (> 25MHZ). Users can change the IdleTime assignment from 0 to
* 1, 2... to effectively slowing down TCK by half, quarter...
*
*********************************************************************************/
void
sclock()
{
    writePort(g_ucPinTCK, 0x01);
    writePort(g_ucPinTCK, 0x00);
}
/********************************************************************************
 *
 * ispVMDelay
 *
 *
 * Users must implement a delay to observe a_usTimeDelay, where
 * bit 15 of the a_usTimeDelay defines the unit.
 *      1 = milliseconds
 *      0 = microseconds
 * Example:
 *      a_usTimeDelay = 0x0001 = 1 microsecond delay.
 *      a_usTimeDelay = 0x8001 = 1 millisecond delay.
 *
 * This subroutine is called upon to provide a delay from 1 millisecond to a few
 * hundreds milliseconds each time.
 * It is understood that due to a_usTimeDelay is defined as unsigned short, a 16 bits
 * integer, this function is restricted to produce a delay to 64000 micro-seconds
 * or 32000 milli-second maximum. The VME file will never pass on to this function
 * a delay time > those maximum number. If it needs more than those maximum, the VME
 * file will launch the delay function several times to realize a larger delay time
 * cummulatively.
 * It is perfectly alright to provide a longer delay than required. It is not
 * acceptable if the delay is shorter.
 *
 * Delay function example--using the machine clock signal of the native CPU------
 * When porting ispVME to a native CPU environment, the speed of CPU or
 * the system clock that drives the CPU is usually known.
 * The speed or the time it takes for the native CPU to execute one for loop
 * then can be calculated as follows:
 *       The for loop usually is compiled into the ASSEMBLY code as shown below:
 *       LOOP: DEC RA;
 *             JNZ LOOP;
 *       If each line of assembly code needs 4 machine cycles to execute,
 *       the total number of machine cycles to execute the loop is 2 x 4 = 8.
 *       Usually system clock = machine clock (the internal CPU clock).
 *       Note: Some CPU has a clock multiplier to double the system clock for
              the machine clock.
 *
 *       Let the machine clock frequency of the CPU be F, or 1 machine cycle = 1/F.
 *       The time it takes to execute one for loop = (1/F ) x 8.
 *       Or one micro-second = F(MHz)/8;
 *
 * Example: The CPU internal clock is set to 100Mhz, then one micro-second = 100/8 = 12
 *
 * The C code shown below can be used to create the milli-second accuracy.
 * Users only need to enter the speed of the cpu.
 *
 **********************************************************************************/
void
ispVMDelay(unsigned short a_usTimeDelay)
{
    unsigned long ulUs = 0;


    /* unit in millisecond, ms */
    if (a_usTimeDelay & 0x8000) {
        ulUs = a_usTimeDelay & 0x7FFF;
        ulUs *= 1000;
        usleep(ulUs);
    }
    /* it is not real microsecond, it will affect sclock */
    else {
        ulUs = a_usTimeDelay & 0x7FFF;
        if (ulUs >= 100) {
            ulUs /= 100;
            usleep(ulUs);
        } else {
            ulUs = 1;
        }

/*        usleep(ulUs); */
    }
}

/*********************************************************************************
 *
 * calibration
 *
 * It is important to confirm if the delay function is indeed providing
 * the accuracy required. Also one other important parameter needed
 * checking is the clock frequency.
 * Calibration will help to determine the system clock frequency
 * and the loop_per_micro value for one micro-second delay of the target
 * specific hardware.
 *
 **********************************************************************************/
void
calibration(void)
{
    /*Apply 2 pulses to TCK.*/
    writePort(g_ucPinTCK, 0x00);
    writePort(g_ucPinTCK, 0x01);
    writePort(g_ucPinTCK, 0x00);
    writePort(g_ucPinTCK, 0x01);
    writePort(g_ucPinTCK, 0x00);

    writePort(g_ucPinTMS, 0x00);
    writePort(g_ucPinTDI, 0x00);
    /*Delay for 1 millisecond. Pass on 1000 or 0x8001 both = 1ms delay.*/
    ispVMDelay(0x8001);
    writePort(g_ucPinTMS, 0x01);
    writePort(g_ucPinTDI, 0x01);

    /*Apply 2 pulses to TCK*/
    writePort(g_ucPinTCK, 0x01);
    writePort(g_ucPinTCK, 0x00);
    writePort(g_ucPinTCK, 0x01);
    writePort(g_ucPinTCK, 0x00);
}

#ifdef __cplusplus
};
#endif