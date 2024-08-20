/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Register.h
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

#pragma once

#include "DevInternal.h"
#include "DevThread.h"
#include "Delay.h"
#include "DevData.h"


/*************************************************/

class CRegister
{
public:
    CRegister()
    {
    }
    CRegister(uint32_t uiIdx) : m_uiDevIdx(uiIdx)
    {
    }
    virtual
    ~CRegister()
    {
    }

    virtual bool Register(void) = 0;
    virtual bool
    Unregister(uint32_t uiDevIdx)
    {
        return true;
    };

protected:
    uint32_t m_uiDevIdx;

};

/* Board */
class CBoardRegister : public CRegister
{
public:
    CBoardRegister(const CBoardRegisterInfo &c_rstInfo) : CRegister(0), m_stRegInfo(c_rstInfo)
    {
    }
    ~CBoardRegister()
    {
    }

    virtual bool
    Register(void) override
    {
        /* check if this dev has been registered */
        auto itIter = ms_staRegList.find(m_uiDevIdx);
        if (ms_staRegList.end() == itIter) {
            std::shared_ptr<CBoardThread> stpBoradThread = std::make_shared<CBoardThread>();
            ms_staRegList[m_uiDevIdx] = stpBoradThread;

            stpBoradThread->SetSubDevInfo(m_stRegInfo);
            stpBoradThread->Start();

            /* wait board init complete because other devices need get driver info */
            /* from board in which there are FPGA and CPLD driver */
            while (!stpBoradThread->GetInitCmpltFlg()) {

                CDelay::Delay(10, MS);
            }
        }

        return true;
    }

    virtual bool
    Unregister(uint32_t uiDevIdx) override
    {
        LOG_WRITE(EN_LOG_NOTICE, "Board Unregister +++");
        auto itIter = ms_staRegList.find(uiDevIdx);
        if (ms_staRegList.end() == itIter) {
            return true;
        }

        if (nullptr == itIter->second) {
            return false;
        }

        itIter->second->ExitThread();
        ms_staRegList.erase(itIter);

        LOG_WRITE(EN_LOG_NOTICE, "Board Unregister ---");
        return true;
    }

private:
    CBoardRegisterInfo m_stRegInfo;

    static std::map<uint32_t, std::shared_ptr<CBoardThread> > ms_staRegList;
};
#if 0
/* all devices register class */
#define CLASS_DEV_REG(ClassName, ThreadName)                  \
                                                              \
    class ClassName : public CRegister                            \
    {                                                             \
public:                                                       \
        ClassName(){}                                             \
        ClassName(const CRegisterInfo &c_rstInfo) :                \
            CRegister(c_rstInfo.uiDevIdx), m_stRegInfo(c_rstInfo){}    \
        ~ClassName(){}                                            \
                                                              \
        virtual bool Register(void) override                      \
        {                                                         \
            if (m_uiDevIdx >= DEV_MAX_NUM)                        \
            {                                                     \
                return false;                                     \
            }                                                     \
            auto itIter = ms_staRegList.find(m_uiDevIdx);         \
            if (ms_staRegList.end() == itIter)                    \
            {                                                     \
                std::shared_ptr<ThreadName> stpDevThread = std::make_shared<ThreadName>(); \
                ms_staRegList[m_uiDevIdx] = stpDevThread;         \
                stpDevThread->SetDevInfo(m_stRegInfo);            \
                stpDevThread->Start();                            \
                while (!stpDevThread->GetInitCmpltFlg())          \
                {                                                 \
                    CDelay::Delay(10, MS);                        \
                }                                                 \
            }                                                     \
            return true;                                          \
        }                                                         \
        virtual bool Unregister(uint32_t uiDevIdx) override       \
        {                                                         \
            auto itIter = ms_staRegList.find(uiDevIdx);           \
            if (ms_staRegList.end() == itIter)                    \
            {                                                     \
                return true;                                      \
            }                                                     \
            if (nullptr == itIter->second)                        \
            {                                                     \
                return false;                                     \
            }                                                     \
            itIter->second->ExitThread();                         \
            ms_staRegList.erase(itIter);                          \
            return true;                                          \
        }                                                         \
                                                              \
private:                                                      \
        CRegisterInfo m_stRegInfo;                                \
        static std::map<uint32_t, std::shared_ptr<ThreadName> > ms_staRegList; \
    }


CLASS_DEV_REG(COaRegister, COaThread);
CLASS_DEV_REG(CWssRegister, CWssThread);
CLASS_DEV_REG(COcmRegister, COcmThread);
CLASS_DEV_REG(CTdcmRegister, CTdcmThread);
CLASS_DEV_REG(CPam4Register, CPam4Thread);
CLASS_DEV_REG(COpsRegister, COpsThread);
CLASS_DEV_REG(CDceRegister, CDceThread);
CLASS_DEV_REG(CDgeRegister, CDgeThread);
CLASS_DEV_REG(CMcsRegister, CMcsThread);
CLASS_DEV_REG(COscRegister, COscThread);
CLASS_DEV_REG(COtdrRegister, COtdrThread);
#endif


template<class T>
class CDevRegister : public CRegister
{
public:
    CDevRegister()
    {
    }
    CDevRegister(const std::vector<CRegisterInfo> &c_rstaInfo, uint32_t uiDevIdx) : CRegister(uiDevIdx), m_staRegInfo(c_rstaInfo)
    {
    }
    ~CDevRegister()
    {
    }

    virtual bool
    Register(void) override
    {
        if (m_uiDevIdx >= DEV_MAX_NUM) {
            return false;
        }

        auto itIter = ms_staRegList.find(m_uiDevIdx);
        if (ms_staRegList.end() == itIter) {
            std::shared_ptr<T> stpDevThread = std::make_shared<T>();
            ms_staRegList[m_uiDevIdx] = stpDevThread;

            stpDevThread->SetDevIdx(m_uiDevIdx);
            stpDevThread->SetDevInfo(m_staRegInfo);
            stpDevThread->Start();

            while (!stpDevThread->GetInitCmpltFlg()) {
                CDelay::Delay(10, MS);
            }
        }

        return true;
    }

    virtual bool
    Unregister(uint32_t uiDevIdx) override
    {

        LOG_WRITE(EN_LOG_NOTICE, "Unregister +++");

        auto itIter = ms_staRegList.find(uiDevIdx);
        if (ms_staRegList.end() == itIter) {
            return true;
        }

        if (nullptr == itIter->second) {
            return false;
        }

        itIter->second->ExitThread();
        ms_staRegList.erase(itIter);


        LOG_WRITE(EN_LOG_NOTICE, "Unregister ---");

        return true;
    }

private:
    std::vector<CRegisterInfo> m_staRegInfo;
    static std::map<uint32_t, std::shared_ptr<T> > ms_staRegList;
};

typedef CDevRegister<COaThread> COaRegister;
typedef CDevRegister<CWssThread> CWssRegister;
typedef CDevRegister<COcmThread> COcmRegister;
typedef CDevRegister<CTdcmThread> CTdcmRegister;
typedef CDevRegister<CPam4Thread> CPam4Register;
typedef CDevRegister<COpsThread> COpsRegister;
typedef CDevRegister<CDgeThread> CDgeRegister;

typedef CDevRegister<CDceThread> CDceRegister;
typedef CDevRegister<CMcsThread> CMcsRegister;
typedef CDevRegister<COscThread> COscRegister;
typedef CDevRegister<COtdrThread> COtdrRegister;
typedef CDevRegister<CMuxThread> CMuxRegister;
typedef CDevRegister<CExtendThread> CExtendRegister;

typedef CDevRegister<COcsThread> COcsRegister;


template<class T>
std::map<uint32_t, std::shared_ptr<T> > CDevRegister<T>::ms_staRegList;

class CRegisterFactory
{
public:

    std::shared_ptr<CRegister> GetRegister(uint32_t uiType);
    std::shared_ptr<CRegister> GetRegister(uint32_t uiType, const std::vector<CRegisterInfo> &c_rstaInfo, uint32_t uiDevIdx);

};




