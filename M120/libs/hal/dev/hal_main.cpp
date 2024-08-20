/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Hal_main.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/
#include <signal.h>
#include <iostream>
#include <fstream>

#include "DevInternal.h"
#include "Delay.h"
#include "Event.h"
#include "HalCmd.h"
#include "DevData.h"
#include "Register.h"
#include "DevCmd.h"
#include "SlotManager.h"
#include "SlotCmd.h"
#include "RegisterFile.h"
#include "LogicI2c.h"

#define HAL_PID_FILE   "/var/run/hal.pid"
static volatile bool m_bExitHalFlag = false;

/*==============================function=========================*/
static void
__InterruptHandler(int iSignal)
{
    LOG_WRITE(EN_LOG_DEBUG, "++ recv signal %d, %s", iSignal, strsignal(iSignal));

    if (SIGPIPE == iSignal) {
    } else if (SIGUSR1 == iSignal) {
    } else {
        LOG_WRITE(EN_LOG_DEBUG, "HAL exit requested, exiting");
        m_bExitHalFlag = true;
    }
}

static std::string
__GetUsbList()
{
    std::string staUsbList(1024, 0);

    char acCmd[64] = {"/usr/local/bin/lsusb"};
    UTIL_ExceCmdUnresp(acCmd, (char *)staUsbList.data(), staUsbList.size(), 0);

    return staUsbList;
}

static int32_t
__UsbDeviceCnt(const std::string &staList, const std::string &staDevice)
{
    int32_t iCnt = 0;

    if (staDevice.empty()) {
        return 0;
    }

    size_t iStart = 0, iIndex = staList.find(staDevice);

    while (iIndex != staList.npos) {
        iCnt++;

        iStart = iIndex + staDevice.size();
        iIndex = staList.find(staDevice, iStart);
    }

    return iCnt;
}

static void
__ReloadUsbCtl()
{

#if 0
    std::ofstream ofile;
    ofile.open("/home/oplink/log/hal-usb.log", std::ios::app);

    ofile << "__ReloadUsbCtl. +++" << std::endl;
    system("lsusb >> /home/oplink/log/hal-usb.log");


    ofile << "__ReloadUsbCtl..." << std::endl;
    system("echo -n 0000:00:1d.0|tee /sys/bus/pci/drivers/ehci-pci/unbind");
    CDelay::Delay(1, S);
    system("echo -n 0000:00:1d.0|tee /sys/bus/pci/drivers/ehci-pci/bind");
    CDelay::Delay(1, S);

    system("lsusb >> /home/oplink/log/hal-usb.log");

    ofile << "__ReloadUsbCtl. ---" << std::endl;
    ofile.close();
#endif

}

#if 0
static void
__UsbListLog()
{
    std::ofstream ofile;
    ofile.open("/home/oplink/log/hal-usb.log", std::ios::app);
    system("lsusb >> /home/oplink/log/hal-usb.log");
    ofile << std::endl;
    ofile.close();
}
#endif

static void
__UsbCheck()
{

#define USB_TO_IIC_DEVICE_CNT   (2)
#define USB_TO_UART_DEVICE_CNT  (1)

#define USB_TO_IIC_STR  "601c Future Technology Devices International, Ltd"
#define USB_TO_UART_STR "6010 Future Technology Devices International, Ltd FT2232C Dual USB-UART/FIFO IC"

    int32_t iUsbToIIC = 0, iUsbToUart = 0;
    std::string staUsbList;

    /*
       Bus 001 Device 005: ID 0403:601c Future Technology Devices International, Ltd
       Bus 001 Device 004: ID 0403:601c Future Technology Devices International, Ltd
       Bus 001 Device 003: ID 0403:6010 Future Technology Devices International, Ltd FT2232C Dual USB-UART/FIFO IC
       Bus 001 Device 002: ID 8087:07e6 Intel Corp.
       Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
     */
    staUsbList = __GetUsbList();

    std::string staUsbToIIC(USB_TO_IIC_STR);
    iUsbToIIC = __UsbDeviceCnt(staUsbList, staUsbToIIC);

    std::string staUsbToUart(USB_TO_UART_STR);
    iUsbToUart = __UsbDeviceCnt(staUsbList, staUsbToUart);

    /* __UsbListLog(); */

    if (USB_TO_IIC_DEVICE_CNT != iUsbToIIC || USB_TO_UART_DEVICE_CNT != iUsbToUart) {
        LOG_WRITE(EN_LOG_ERROR, "iUsbToIIC = %d, iUsbToUart = %d", iUsbToIIC, iUsbToUart);
        __ReloadUsbCtl();
    }
}

/* There are muti RegisterFiles in /etc/ directory which is from HAL repository, we need to read  */
/* MFGINFO from chassis eeprom and using the specific RegisterFile-XXX.                           */
/*   Product NR1004, RegisterFile-NR1004                                                          */
/*   Product OLSP,   RegisterFile-OLSP                                                            */
/*   Product OPS4,   RegisterFile-OPS4                                                            */
/*   default,        RegisterFile-NR1004                                                          */
#define DEF_MODEL_OPS4   "OPS-4"
#define DEF_MODEL_NR1004 "NR1004"
#define DEF_MODEL_OLSP   "OLS-P"
#define DEF_REGISTER_FILE        "/etc/RegisterFile"
#define DEF_REGISTER_FILE_OPS4   "/etc/RegisterFile-OPS4"
#define DEF_REGISTER_FILE_NR1004 "/etc/RegisterFile-NR1004"
#define DEF_REGISTER_FILE_NR1004_VEVICE "/etc/RegisterFile-NR1004-vevice"
#define DEF_REGISTER_FILE_OLSP   "/etc/RegisterFile-OLSP"
#define DEF_REGISTER_FILE_OLSP_VEVICE   "/etc/RegisterFile-OLSP-vevice"
#define DEF_REGISTER_FILE_HCF   "/etc/RegisterFile-HCF"
#define DEF_REGISTER_FILE_HCF_VEVICE   "/etc/RegisterFile-HCF-vevice"
#define DEF_REGISTER_FILE_OCS     "/etc/RegisterFile-OCS"

bool
__ChassisTypeCheck()
{
#define EEPHEADOFFSET                32

    std::shared_ptr<CLogicDriver> stpLogicDrvPtr = nullptr;
    std::shared_ptr<CI2c> stpI2cPtr = nullptr;
    std::shared_ptr<CEEprom> stpEEpromPtr = nullptr;
    int iRet = OPLK_ERR;
    char acRegisterCmd[DEF_BUF_SIZE_128] = {0};

    /* Get mfginfo from chassis EEPROM */
    uint8_t aucBuf[8192] = {0};
    CMfg stMfgInfo;

    memset(&stMfgInfo, 0, sizeof(stMfgInfo));

    LOG_WRITE(EN_LOG_NOTICE, "Start check chassis type");

    stpLogicDrvPtr = std::static_pointer_cast<CLogicDriver>(
        std::make_shared<CPcieBusLogic>(0, 0, 0, DATA_WIDTH_1B));
    if (nullptr == stpLogicDrvPtr) {
        LOG_WRITE(EN_LOG_ERROR, "stpLogicDrvPtr is nullptr.");
        goto END;
    }

    stpI2cPtr = std::static_pointer_cast<CI2c>(
        std::make_shared<CLogicI2c>(stpLogicDrvPtr, 128));
    if (nullptr == stpI2cPtr) {
        LOG_WRITE(EN_LOG_ERROR, "stpI2cPtr is nullptr.");
        goto END;
    }


    stpEEpromPtr = std::static_pointer_cast<CEEprom>
                       (std::make_shared<CI2cEEprom>(stpI2cPtr, 0, 172));

    if (nullptr == stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "stpEEpromPtr is nullptr.");
        goto END;
    }

    if (false == stpEEpromPtr->Read(0, (UCHAR *)&aucBuf, sizeof(aucBuf))) {
        LOG_WRITE(EN_LOG_ERROR, "Read rpc Eprom error.");
        goto END;
    }
    memcpy(&stMfgInfo, &aucBuf[EEPHEADOFFSET], sizeof(stMfgInfo));
#if 0
    UCHAR *pucBuf = NULL;
    pucBuf = (UCHAR *)&stMfgInfo;
    printf("len:%d\r\n", (int)sizeof(stMfgInfo));
    for (int i = 0; i < (int)sizeof(stMfgInfo); i++) {
        printf("%02X ", pucBuf[i]);

        if (0 == (i + 1) % 16) {
            printf("\r\n");
        }
    }
    printf("\r\n");
#endif
END:

    LOG_WRITE(EN_LOG_NOTICE, "Productname: %s, config type: %s", stMfgInfo.acProductName, stMfgInfo.acConfigType);
#if defined(__OCS__)
    snprintf(acRegisterCmd, sizeof(acRegisterCmd), "cp %s %s", DEF_REGISTER_FILE_OCS, DEF_REGISTER_FILE);
#elif defined(__HCF__)
    /* if (0 == strcmp(stMfgInfo.acProductName, "HCF")) {    STELLWAGEN */
#ifdef _VEVICE_
    snprintf(acRegisterCmd, sizeof(acRegisterCmd), "cp %s %s", DEF_REGISTER_FILE_HCF_VEVICE, DEF_REGISTER_FILE);
#else
    snprintf(acRegisterCmd, sizeof(acRegisterCmd), "cp %s %s", DEF_REGISTER_FILE_HCF, DEF_REGISTER_FILE);
#endif
    /* } */
#else
    if (0 == strcmp(stMfgInfo.acProductName, "OLS-P")) {
    #ifdef _VEVICE_
        snprintf(acRegisterCmd, sizeof(acRegisterCmd), "cp %s %s", DEF_REGISTER_FILE_OLSP_VEVICE, DEF_REGISTER_FILE);
    #else
        snprintf(acRegisterCmd, sizeof(acRegisterCmd), "cp %s %s", DEF_REGISTER_FILE_OLSP, DEF_REGISTER_FILE);
    #endif
    } else {
        /* Using config type to seperate AMP or OPS-4                           */
        if (0 == strcmp(stMfgInfo.acConfigType, "OPS")) {
            snprintf(acRegisterCmd, sizeof(acRegisterCmd), "cp %s %s", DEF_REGISTER_FILE_OPS4, DEF_REGISTER_FILE);
        } else {
            /* Using default value 'NR1004'                                     */
        #ifdef _VEVICE_
            snprintf(acRegisterCmd, sizeof(acRegisterCmd), "cp %s %s", DEF_REGISTER_FILE_NR1004_VEVICE, DEF_REGISTER_FILE);
        #else
            snprintf(acRegisterCmd, sizeof(acRegisterCmd), "cp %s %s", DEF_REGISTER_FILE_NR1004, DEF_REGISTER_FILE);
        #endif
        }
    }
#endif
    LOG_WRITE(EN_LOG_NOTICE, "Exec '%s'", acRegisterCmd);
    iRet = UTIL_System(acRegisterCmd);
    if (OPLK_OK != iRet) {
        LOG_WRITE(EN_LOG_ERROR, "system cmd error:%s", acRegisterCmd);
        return false;
    }

    return true;
}


int32_t
main(int32_t argc, char *argv[])
{
    int32_t iPidFd = -1;

    UTIL_InitClientLog();

    LOG_WRITE(EN_LOG_NOTICE, "HAL main start");
    __ChassisTypeCheck();
    /* usb load issue workaround */
    __UsbCheck();

    /* 1. register board and devices on it */
    CEvent::CreateSocket();

    UTIL_HeartbeatCfgEnalbe(0);

    bool bRet = CRegisterFile::Load();
    if (false == bRet) {
        LOG_WRITE(EN_LOG_ERROR, "Register File Load error.");
        /* mantis-39816 when process init error, the heart-beat checking won't be enabled. */
        /* So we need to set heart-beat enable when init error.                            */
        /* In case of initialization time too long, disable the heart-beat when startup    */
        /* @ {                                                                             */
        UTIL_HeartbeatCfgEnalbe(1);
        /* @ }                                                                             */
        return -1;
    }

    const std::vector<CRegisterInfo> c_stOscRegInfo =
    {
        /* OSC_IDX_0 */
        {
            {HW_TYPE_OSC_BOARD, OSC_BOARD0, OSC_IDX_0, 0, PROTO_TYPE_DEF}
        }
    };

    const std::vector<CRegisterInfo> c_stOsc1RegInfo =
    {
        {
            {HW_TYPE_OSC_BOARD, OSC_BOARD0, OSC_IDX_1, 0, PROTO_TYPE_DEF}
        }
    };


    /* const CRegisterInfo c_stMuxRegInfo = {MUX_DEMUX, HW_TYPE_SPI_USB_FT, 0, 2, 0, PROTO_TYPE_DEF}; */


    /* device list */
    const std::vector<std::shared_ptr<CRegister> > c_staRegisters =
    {
        /* board must be registered first because CPLD and FPGA on it */
        /* are used to drive other devices */
        std::static_pointer_cast<CRegister>(std::make_shared<CBoardRegister>(CRegisterFile::ms_stBoardRegInfo)),

        /* modules */
        /* std::static_pointer_cast<CRegister>(std::make_shared<COscRegister>(c_stOsc0RegInfo, 0)), */

        /* std::static_pointer_cast<CRegister>(std::make_shared<COscRegister>(c_stOsc1RegInfo, 1)), */
        /* std::static_pointer_cast<CRegister>(std::make_shared<CMuxRegister>(c_stMuxRegInfo)), */

    };

    for (auto pstRegister : c_staRegisters) {
        pstRegister->Register();
    }

/* 2. register all slots */
    CSlotManager::GetInstance().CreateShm();
    CSlotManager::GetInstance().Register(CRegisterFile::ms_staSlotRegInfo);

/* 3. loop */
    LOG_WRITE(EN_LOG_NOTICE, "HAL main is running");
    printf("HAL main is running\n");


    signal(SIGINT, __InterruptHandler);
    signal(SIGKILL, __InterruptHandler);
    signal(SIGTERM, __InterruptHandler);
    signal(SIGUSR1, __InterruptHandler);
    signal(SIGPIPE, __InterruptHandler);

    UTIL_CreatePidFile(HAL_PID_FILE, &iPidFd);

    UTIL_HeartbeatCfgChkTimeout(120);

    uint32_t ulCount = 0;
    prctl(PR_SET_NAME, "hal_MainThread");
    /* while (!m_bExitHalFlag) { */
    while (true) {
        LOG_WRITE(EN_LOG_TRACE, "m_bExitHalFlag = %d, count = %u", m_bExitHalFlag, ulCount);

        /* slot management */
        CSlotManager::GetInstance().Manage(m_bExitHalFlag);

        if (m_bExitHalFlag) {
            break;
        }
        CDelay::Delay(1, S);

        if (0 == ulCount) {
            UTIL_HeartbeatCfgEnalbe(1);
            UTIL_HeartbeatCfgChkTimeout(60);
        }

        if (UTIL_CheckThreadHeartBeart()) {
            UTIL_SendHeartbeat();
        }

        ulCount++;
    }

    for (auto pstRegister : c_staRegisters) {
        pstRegister->Unregister(0);
    }

    LOG_WRITE(EN_LOG_NOTICE, "HAL main end");
    CEvent::CloseSocket();
    UTIL_CloseHeartBeat();
    UTIL_DelPidFile(HAL_PID_FILE, iPidFd);
    UTIL_CloseClientLog();
    return 0;

}
