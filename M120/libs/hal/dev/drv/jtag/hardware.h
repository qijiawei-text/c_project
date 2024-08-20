/********************************************************************************
 * Declaration of global variables
 *
 *********************************************************************************/


/*********************************************************************************
* This is the definition of the bit locations of each respective
* signal in the global variable g_siIspPins.
*
* NOTE: Users must add their own implementation here to define
*       the bit location of the signal to target their hardware.
*       The example below is for the Lattice download cable on
*       on the parallel port.
*
*********************************************************************************/

const unsigned char g_ucPinTDI = 0x01;             /* Bit address of TDI */
const unsigned char g_ucPinTCK = 0x02;             /* Bit address of TCK */
const unsigned char g_ucPinTMS = 0x04;             /* Bit address of TMS */
const unsigned char g_ucPinENABLE = 0x08;          /* Bit address of ENABLE */
const unsigned char g_ucPinTRST = 0x10;            /* Bit address of TRST */
const unsigned char g_ucPinTDO = 0x40;             /* Bit address of TDO*/
