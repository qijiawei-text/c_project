/**************************************************************
 *
 * Lattice Semiconductor Corp. Copyright 2008
 *
 * This ivm_eprom.c is the alternative module to ispvm_ui.c of the
 * ispVME Embedded C Source. When using this module, the VME file is
 * compiled and linked with ivm_core.c and hardware.c to become a
 * single executable.
 *
 * ispVME Embedded C Source comprised with 3 modules:
 * ivm_eprom.c is the module provides input and output support.
 * ivm_core.c is the module interpret the VME file(s).
 * hardware.c is the module access the JTAG port of the device(s).
 *
 * The optional module cable.c is for supporting Lattice's parallel port
 * ispDOWNLOAD cable on DOS and Windows 95/98 O/S. It can be requested
 * from Lattice's ispVMSupport.
 *
 ***************************************************************/


/**************************************************************
 *
 * Revision History of ispvm_ui.c
 *
 * 3/6/07 ht Added functions vme_out_char(),vme_out_hex(),
 *           vme_out_string() to provide output resources.
 *           Consolidate all printf() calls into the added output
 *           functions.
 *
 * 09/11/07 NN Added Global variables initialization
 * 09/19/07 NN Removed g_usDelayPercent option
 * 11/15/07  NN moved the checking of the File CRC to the end of processing
 * 11/15/07  NN moved the checking of the File CRC to the end of processing
 * 08/28/08 NN Added Calculate checksum support.
 ***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vmopcode.h"
#include "Jtag.h"

#ifdef __cplusplus
extern "C" {
#endif


/***************************************************************
*
* Global variable that points to the current VME data array.
*
***************************************************************/

/* unsigned char *g_pDataPointer = NULL; */
/* unsigned short ucVMEArrayIndex = 0; */
/* unsigned short usDataIndex     = 0; */

CJtag *g_pJtag = nullptr;

unsigned short g_usExpectedCRC = 0;
unsigned short g_usCaculate = 0;

/* ispVM Embedded File for ispVM                                                                  */
/*-----------------caution------------------------------------------------------------------------*/
/* every vme file need dirrent size, so reserve few ram for every Buf                             */
/* please check every times when you need upgrade lattice cpld (No needed in PPC)                 */
/*-----------------caution------------------------------------------------------------------------*/
static unsigned char m_aucMaskBuf[32];                                      /* Memory to hold MASK data.   >= 5                        */
static unsigned char m_aucTdiBuf[256];                                      /* Memory to hold TDI data.    >= 54                       */
static unsigned char m_aucTdoBuf[256];                                      /* Memory to hold TDO data.    >= 54                       */
static unsigned char *m_pucHdrBuf = NULL;                                   /* Memory to hold HDR data.                                */
static unsigned char *m_pucTdrBuf = NULL;                                   /* Memory to hold TDR data.                                */
static unsigned char *m_pucHirBuf = NULL;                                   /* Memory to hold HIR data.                                */
static unsigned char *m_pucTirBuf = NULL;                                   /* Memory to hold TIR data.                                */
static unsigned char m_aucHeapBuf[128];                                     /* Memory to hold HEAP data.                               */
static unsigned char *m_pucDMASKBuf = NULL;                                 /* Memory to hold DMASK data.                              */
static unsigned char m_aucLCOUNTBuf[128];                                   /* Memory to hold LCOUNT data. >= 17                       */
static LVDSPair *m_pstLVDSBuf = NULL;                                       /* Memory to hold LVDS Pair.                               */
/* ispVM Embedded File for ispVM                                                                  */

/***************************************************************
*
* functions declared in this module.
*
***************************************************************/
unsigned char GetByte();
signed char ispVM(CJtag *pJtag);
void ispVMMemManager(signed char cTarget, unsigned short usSize);
void error_handler(short a_siRetCode, char *pszMessage);
void vme_out_char(unsigned char charOut);
void vme_out_hex(unsigned char hexOut);
void vme_out_string(const char *stringOut);


/***************************************************************
*
* functions and variables declared in ivm_core.c.
*
***************************************************************/
extern signed char ispVMCode();
extern void ispVMCalculateCRC32(unsigned char a_ucData);
extern void ispVMStart();
extern void ispVMEnd();
extern unsigned short g_usCalculatedCRC;
extern unsigned short g_usDataType;
extern unsigned char *g_pucOutMaskData,
*g_pucInData,
*g_pucOutData,
*g_pucHIRData,
*g_pucTIRData,
*g_pucHDRData,
*g_pucTDRData,
*g_pucOutDMaskData,
*g_pucIntelBuffer;
extern CJtag *g_pJtag;
extern unsigned char *g_pucHeapMemory;
extern unsigned short g_iHeapCounter;
extern unsigned short g_iHEAPSize;
extern unsigned short g_usIntelDataIndex;
extern unsigned short g_usIntelBufferSize;
extern LVDSPair *g_pLVDSList;
/* 08/28/08 NN Added Calculate checksum support. */
extern unsigned long g_usChecksum;
extern unsigned int g_uiChecksumIndex;
/***************************************************************
*
* Supported VME versions.
*
***************************************************************/

const char *const g_szSupportedVersions[] = {"__VME2.0", "__VME3.0", "____12.0", "____12.1", 0};

/***************************************************************
*
* GetByte
*
* Returns a byte to the caller. The returned byte depends on the
* g_usDataType register. If the HEAP_IN bit is set, then the byte
* is returned from the HEAP. If the LHEAP_IN bit is set, then
* the byte is returned from the intelligent buffer. Otherwise,
* the byte is returned directly from the VME file.
*
***************************************************************/

unsigned char
GetByte()
{
    unsigned char ucData = 0;

    if (g_usDataType & HEAP_IN) {

        /***************************************************************
        *
        * Get data from repeat buffer.
        *
        ***************************************************************/

        if (g_iHeapCounter > g_iHEAPSize) {

            /***************************************************************
            *
            * Data over-run.
            *
            ***************************************************************/

            return 0xFF;
        }

        ucData = g_pucHeapMemory[g_iHeapCounter++];
    } else if (g_usDataType & LHEAP_IN) {

        /***************************************************************
        *
        * Get data from intel buffer.
        *
        ***************************************************************/

        if (g_usIntelDataIndex >= g_usIntelBufferSize) {

            /***************************************************************
            *
            * Data over-run.
            *
            ***************************************************************/

            return 0xFF;
        }

        ucData = g_pucIntelBuffer[g_usIntelDataIndex++];
    } else {

        /***************************************************************
        *
        * Get data from the compiled arrays.
        *
        ***************************************************************/
        /* CJtag::GetImg(&ucData); */
        g_pJtag->GetImg(&ucData);
#if 0
        if (usDataIndex == 0) {
            g_pDataPointer = vmeArray[ucVMEArrayIndex++];
        }

        ucData = *g_pDataPointer++;
        if (usDataIndex < (unsigned short)((g_pArraySizes[ucVMEArrayIndex - 1]) - 1)) {
            usDataIndex++;
        } else {
            usDataIndex = 0;
            if (ucVMEArrayIndex == (unsigned short)(vmearrays)) {
                ucVMEArrayIndex = 0;
            }
        }
#endif

        /***************************************************************
        *
        * Calculate the 32-bit CRC if the expected CRC exist.
        *
        ***************************************************************/
        if (g_usCaculate != 0) {
            ispVMCalculateCRC32(ucData);
        }
    }

    return ucData;
}
/***************************************************************
*
* vme_out_char
*
* Send a character out to the output resource if available.
* The monitor is the default output resource.
*
*
***************************************************************/
void
vme_out_char(unsigned char charOut)
{
    printf("%c", charOut);
}
/***************************************************************
*
* vme_out_hex
*
* Send a character out as in hex format to the output resource
* if available. The monitor is the default output resource.
*
*
***************************************************************/
void
vme_out_hex(unsigned char hexOut)
{
    printf("%.2X", hexOut);
}
/***************************************************************
*
* vme_out_string
*
* Send a text string out to the output resource if available.
* The monitor is the default output resource.
*
*
***************************************************************/
void
vme_out_string(const char *stringOut)
{
    if (stringOut) {
        printf("%s", stringOut);
    }

}
/***************************************************************
*
* ispVM
*
* The entry point of the ispVM embedded. If the version and CRC
* are verified, then the VME will be processed.
*
***************************************************************/

signed char
ispVM(CJtag *pJtag)
{
    signed char szFileVersion[9] = {0};
    signed char cRetCode = 0;
    signed char cIndex = 0;
    signed char cVersionIndex = 0;
    unsigned char ucReadByte = 0;

    /***************************************************************
    *
    * Global variables initialization.
    *
    * 09/11/07 NN Added
    ***************************************************************/
/*    ucVMEArrayIndex = 0; */
/*    usDataIndex = 0; */

    g_pJtag = pJtag;
    g_pucHeapMemory = NULL;
    g_iHeapCounter = 0;
    g_iHEAPSize = 0;
    g_usIntelDataIndex = 0;
    g_usIntelBufferSize = 0;

    g_usCalculatedCRC = 0;
    g_usExpectedCRC = 0;
    g_usCaculate = 0;
    ucReadByte = GetByte();
    switch (ucReadByte) {
    case FILE_CRC:

        /***************************************************************
        *
        * Read and store the expected CRC to do the comparison at the end.
        * Only versions 3.0 and higher support CRC protection.
        *
        ***************************************************************/

        g_usExpectedCRC = GetByte();
        g_usExpectedCRC <<= 8;
        g_usExpectedCRC |= GetByte();
        g_usCaculate = 1;

        /***************************************************************
        *
        * Read and store the version of the VME file.
        *
        ***************************************************************/

        for (cIndex = 0; cIndex < 8; cIndex++) {
            szFileVersion[cIndex] = GetByte();
        }

        break;
    default:

        /***************************************************************
        *
        * Read and store the version of the VME file.  Must be version 2.0.
        *
        ***************************************************************/

        szFileVersion[0] = (signed char)ucReadByte;
        for (cIndex = 1; cIndex < 8; cIndex++) {
            szFileVersion[cIndex] = GetByte();
        }

        break;
    }

    /***************************************************************
    *
    * Compare the VME file version against the supported version.
    *
    ***************************************************************/

    for (cVersionIndex = 0; g_szSupportedVersions[cVersionIndex] != 0; cVersionIndex++) {
        for (cIndex = 0; cIndex < 8; cIndex++) {
            if (szFileVersion[cIndex] != g_szSupportedVersions[cVersionIndex][cIndex]) {
                cRetCode = VME_VERSION_FAILURE;
                break;
            }
            cRetCode = 0;
        }

        if (cRetCode == 0) {

            /***************************************************************
            *
            * Found matching version, break.
            *
            ***************************************************************/

            break;
        }
    }

    if (cRetCode < 0) {

        /***************************************************************
        *
        * VME file version failed to match the supported versions.
        *
        ***************************************************************/

        return VME_VERSION_FAILURE;
    }

    /***************************************************************
    *
    * Enable the Lattice cable.
    *
    ***************************************************************/

    ispVMStart();

    /***************************************************************
    *
    * Process the VME file.
    *
    ***************************************************************/

    cRetCode = ispVMCode();

    /***************************************************************
    *
    * Disable the Lattice cable.
    *
    ***************************************************************/

    ispVMEnd();

    /***************************************************************
    *
    * Compare the expected CRC versus the calculated CRC.
    *
    ***************************************************************/

    if (cRetCode == 0 && g_usExpectedCRC != 0 && (g_usExpectedCRC != g_usCalculatedCRC)) {
        printf("Expected CRC:   0x%.4X\n", g_usExpectedCRC);
        printf("Calculated CRC: 0x%.4X\n", g_usCalculatedCRC);
        return VME_CRC_FAILURE;
    }

    return cRetCode;
}

/***************************************************************
*
* ispVMMemManager
*
* Set pointer to the data from the VME hex files.
*
***************************************************************/

void
ispVMMemManager(signed char cTarget, unsigned short usSize)
{
    usSize = 0;
    switch (cTarget) {
    case XTDI:
    case TDI:
        g_pucInData = m_aucTdiBuf;
    case XTDO:
    case TDO:
        g_pucOutData = m_aucTdoBuf;
        break;
    case MASK:
        g_pucOutMaskData = m_aucMaskBuf;
        break;
    case HIR:
        g_pucHIRData = m_pucHirBuf;
        break;
    case TIR:
        g_pucTIRData = m_pucTirBuf;
        break;
    case HDR:
        g_pucHDRData = m_pucHdrBuf;
        break;
    case TDR:
        g_pucTDRData = m_pucTdrBuf;
        break;
    case HEAP:
        g_pucHeapMemory = m_aucHeapBuf;
        break;
    case DMASK:
        g_pucOutDMaskData = m_pucDMASKBuf;
        break;
    case LHEAP:
        g_pucIntelBuffer = m_aucLCOUNTBuf;
        break;
    case LVDS:
        g_pLVDSList = m_pstLVDSBuf;
        break;
    default:
        return;
    }
}
/***************************************************************
*
* error_handler
*
* Reports the error message.
*
***************************************************************/

void
error_handler(short a_siRetCode, char *pszMessage)
{
    const char *pszErrorMessage[] = {"pass",
                                     "verification fail",
                                     "can't find the file",
                                     "wrong file type",
                                     "file error",
                                     "option error",
                                     "crc verification error"};

    strcpy(pszMessage, pszErrorMessage[-a_siRetCode]);
}

/***************************************************************
*
* main
*
***************************************************************/
#if 0
int
main(int argc, signed char *argv[])
{
    signed short siRetCode = 0;
    signed char szCommandLineArg[300] = {0};

    /* 08/28/08 NN Added Calculate checksum support. */
    g_usChecksum = 0;
    g_uiChecksumIndex = 0;


    vme_out_string("\n                 Lattice Semiconductor Corp.\n");
    vme_out_string("\n               ispVME V");
    vme_out_string(VME_VERSION_NUMBER);
    vme_out_string(" Copyright 1998-2011.\n");
    vme_out_string("\n      For Daisy Chain of All In-System Programmable Devices\n\n");

    if (argc != 1) {
        vme_out_string("\nUsage: vme \n\n");
        vme_out_string("Example: vme\n");
        exit(1);
    }
    siRetCode = ispVM();
    if (siRetCode < 0) {
        error_handler(siRetCode, szCommandLineArg);
        vme_out_string("Failed due to ");
        vme_out_string(szCommandLineArg);
        vme_out_string("\n\n");
        vme_out_string("+=======+\n");
        vme_out_string("| FAIL! |\n");
        vme_out_string("+=======+\n\n");
    } else {
        vme_out_string("+=======+\n");
        vme_out_string("| PASS! |\n");
        vme_out_string("+=======+\n\n");
        /* 08/28/08 NN Added Calculate checksum support. */
        if (g_usChecksum != 0) {
            g_usChecksum &= 0xFFFF;
            printf("Data Checksum: %.4X\n\n", g_usChecksum);
            g_usChecksum = 0;
        }
    }
    return siRetCode;
}
#endif

#ifdef __cplusplus
};
#endif


