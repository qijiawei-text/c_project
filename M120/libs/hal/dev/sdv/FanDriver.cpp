#include "FanDriver.h"
#include "EEpromOperator.h"

const uint32_t c_uiMaxFanSpeed = 100;

#if 0
const std::vector<uint32_t> CFan::msc_staRegs =
{
    FPGA_MAIN_REG_FAN_CTRL1,
    FPGA_MAIN_REG_FAN_CTRL2,
    FPGA_MAIN_REG_FAN_CTRL3,
    FPGA_MAIN_REG_FAN_CTRL4,
};

const std::vector<uint32_t> CFan::msc_staOnlineMask =
{
    FPGA_MAIN_FAN0_ONLINE_MASK,
    FPGA_MAIN_FAN1_ONLINE_MASK,
    FPGA_MAIN_FAN2_ONLINE_MASK,
    FPGA_MAIN_FAN3_ONLINE_MASK,
};
#endif

bool
CFan::GetMfg(CMfg &rstMfg)
{

    LOG_WRITE(EN_LOG_NOTICE, "Fan(%d) GetMfg +++", m_uiFanBoardIdx);
    memset(&rstMfg, 0, sizeof(rstMfg));

    if (nullptr == m_stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "fan #%d m_stpEEpromPtr is nullptr. ", m_uiFanBoardIdx);
        return false;
    }

    if ( false == CEEpromOperator::ReadMfg(m_stpEEpromPtr, rstMfg)) {
        LOG_WRITE(EN_LOG_ERROR, "Read fan(%d) mfg error.", m_uiFanBoardIdx);
        return false;
    }

    LOG_WRITE(EN_LOG_NOTICE, "acManufacturer %s", rstMfg.acManufacturer);
    LOG_WRITE(EN_LOG_NOTICE, "acPN %s", rstMfg.acPN);
    LOG_WRITE(EN_LOG_NOTICE, "acSN %s", rstMfg.acSN);
    LOG_WRITE(EN_LOG_NOTICE, "acMfgDate %s", rstMfg.acMfgDate);
    LOG_WRITE(EN_LOG_NOTICE, "acCaliDate %s", rstMfg.acCaliDate);
    LOG_WRITE(EN_LOG_NOTICE, "acBootVer %s", rstMfg.acBootVer);
    LOG_WRITE(EN_LOG_NOTICE, "acFwVer %s", rstMfg.acFwVer);
    LOG_WRITE(EN_LOG_NOTICE, "acHwVer %s", rstMfg.acHwVer);
    LOG_WRITE(EN_LOG_NOTICE, "acModuleType %s", rstMfg.acModuleType);


    LOG_WRITE(EN_LOG_NOTICE, "Fan(%d) GetMfg ---", m_uiFanBoardIdx);

    return true;

}

bool
CFan::SetMfg(const char *c_pcFileName)
{
    LOG_WRITE(EN_LOG_DEBUG, "CFan::SetMfg +++");
    if (NULL == c_pcFileName) {
        LOG_WRITE(EN_LOG_ERROR, "fan #%d set mfg c_pcFileName is NULL. ", m_uiFanBoardIdx);
    }

    if (nullptr == m_stpEEpromPtr) {
        LOG_WRITE(EN_LOG_ERROR, "fan #%d m_stpEEpromPtr is nullptr. ", m_uiFanBoardIdx);
        return false;
    }


    if ( false == CEEpromOperator::WriteMfg(m_stpEEpromPtr, c_pcFileName)) {
        LOG_WRITE(EN_LOG_ERROR, "Write fan(%d) mfg error.", m_uiFanBoardIdx);
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CFan::SetMfg ---");

    return true;

}



bool
CFan::IsOnline(void)
{
    bool bOnline = false;


    LOG_WRITE(EN_LOG_DEBUG, "CFan::IsOnline +++");

    if (nullptr == m_stpFanBoardPtr) {
        LOG_WRITE(EN_LOG_ERROR, "fan #%d driver ptr is nullptr", m_uiFanBoardIdx);
        return false;
    }

    bOnline = m_stpFanBoardPtr->IsOnline(m_uiFanBoardIdx);

    return bOnline;

}

bool
CFan::IsPlug(void)
{
    bool bPlug = false;

    LOG_WRITE(EN_LOG_DEBUG, "Is plug +++");

    if (nullptr == m_stpFanBoardPtr) {
        LOG_WRITE(EN_LOG_ERROR, "fan #%d driver ptr is nullptr", m_uiFanBoardIdx);
        return false;
    }

    bPlug = m_stpFanBoardPtr->IsPlug(m_uiFanBoardIdx);

    LOG_WRITE(EN_LOG_DEBUG, "Is plug ---");

    return bPlug;
}

bool
CFan::ClearPlug(void)
{
    if (nullptr == m_stpFanBoardPtr) {
        LOG_WRITE(EN_LOG_ERROR, "fan #%d driver ptr is nullptr", m_uiFanBoardIdx);
        return false;
    }

    if (false == m_stpFanBoardPtr->ClearPlug(m_uiFanBoardIdx)) {
        LOG_WRITE(EN_LOG_ERROR, "fan #%d clear plug flag error.", m_uiFanBoardIdx);
        return false;
    }

    return true;
}


bool
CFan::GetStatusData(CFanSta & rstData)
{
    bool bRet = false;
    rstData.bIsOnline = IsOnline();

    if (rstData.bIsOnline) {

        GetResetCause(rstData.ucResetCause);
        for (uint32_t uiTimes = 0; uiTimes < 16; uiTimes++) {
            bRet = GetSpeed(rstData.uiSpeed);
            if (bRet) {
                break;
            }
            CDelay::Delay(10, MS);
        }
        if (false == bRet) {
            rstData.bIsFailure = true;
            rstData.uiSpeed = c_uiMaxFanSpeed;

            LOG_WRITE(EN_LOG_ERROR, "get fan #%d speed error.", m_uiFanBoardIdx);

            return false;
        }
#if 0
        /* LOG_WRITE(EN_LOG_DEBUG, "fan #%d rstData.uiSpeed = %d", m_uiHwSubIdx, rstData.uiSpeed); */
        if (false == GetLed(rstData.uiLedState)) {

            LOG_WRITE(EN_LOG_ERROR, "get fan #%d led error.", m_uiFanBoardIdx);

            return false;
        }
#endif

    } else {
        /* LOG_WRITE(EN_LOG_ERROR, "fan #%d offline", m_uiHwSubIdx); */
    }

    rstData.bIsFailure = false;

    return true;
}


bool
CFan::GetSpeed(uint32_t &uiSpeed)
{

    /* LOG_WRITE(EN_LOG_DEBUG, "CFan::GetSpeed +++"); */

    if (nullptr == m_stpMax31760Ptr) {

        LOG_WRITE(EN_LOG_ERROR, "fan #%d driver is nullptr", m_uiFanBoardIdx);
        return false;
    }

    if (false == m_stpMax31760Ptr->GetFanSpeed(uiSpeed)) {
        LOG_WRITE(EN_LOG_ERROR, "fan #%d read error.", m_uiFanBoardIdx);
        return false;
    }

    /* LOG_WRITE(EN_LOG_DEBUG, "CFan::GetSpeed ---"); */

    return true;
}


bool
CFan::SetSpeed(uint32_t uiSpeed)
{
    uint8_t ucSpeed = c_uiMaxFanSpeed;
    uint8_t ucResetCause = 0;
    bool bResetCauseReturn = false;

    ucSpeed = (uint8_t)uiSpeed;

    if (nullptr == m_stpMax31760Ptr) {
        LOG_WRITE(EN_LOG_ERROR, "Fan Board Ptr is nullptr.");
        return false;
    }

    bResetCauseReturn = m_stpMax31760Ptr->GetResetCause(ucResetCause);

    if (false == m_stpMax31760Ptr->SetFanSpeed(ucSpeed)) {
        LOG_WRITE(EN_LOG_ERROR, "Set Fan(%d) Speed(%d) error.", m_uiFanBoardIdx, ucSpeed);
        return false;
    }

    if (true == bResetCauseReturn) {
        m_stpMax31760Ptr->SetResetCause(ucResetCause);
    }

    return true;
}

bool
CFan::GetResetCause(uint8_t &ucResetCause)
{

    /* LOG_WRITE(EN_LOG_DEBUG, "CFan::GetSpeed +++"); */

    if (nullptr == m_stpMax31760Ptr) {

        LOG_WRITE(EN_LOG_ERROR, "fan #%d driver is nullptr", m_uiFanBoardIdx);
        return false;
    }

    if (false == m_stpMax31760Ptr->GetResetCause(ucResetCause)) {
        LOG_WRITE(EN_LOG_ERROR, "fan #%d read reset cause error.", m_uiFanBoardIdx);
        return false;
    }

    /* LOG_WRITE(EN_LOG_DEBUG, "CFan::GetSpeed ---"); */

    return true;
}


bool
CFan::SetResetCause(uint8_t ucResetCause)
{

    if (nullptr == m_stpMax31760Ptr) {
        LOG_WRITE(EN_LOG_ERROR, "Fan Board Ptr is nullptr.");
        return false;
    }

    if (false == m_stpMax31760Ptr->SetResetCause(ucResetCause)) {
        LOG_WRITE(EN_LOG_ERROR, "Set Fan(%d) reset-cause(%d) error.", m_uiFanBoardIdx, ucResetCause);
        return false;
    }

    return true;
}

#if 0
bool
CFan::GetLed(uint32_t &uiLed)
{

    LOG_WRITE(EN_LOG_DEBUG, "CFan::GetLedState +++");

    if (nullptr == m_stpFanBoardPtr) {

        LOG_WRITE(EN_LOG_ERROR, "fan #%d driver is nullptr", m_uiFanBoardIdx);
        return false;
    }

    if (false == m_stpFanBoardPtr->GetFanLedState(m_uiFanBoardIdx, uiLed)) {
        LOG_WRITE(EN_LOG_ERROR, "fan #%d read led state error.", m_uiFanBoardIdx);
        return false;
    }

    LOG_WRITE(EN_LOG_DEBUG, "CFan::GetLedState ---");

    return true;
}


bool
CFan::SetLed(uint32_t uiLed)
{
    uint8_t ucLedState = 0;

    ucLedState = (uint8_t)uiLed;

    if (nullptr == m_stpFanBoardPtr) {
        LOG_WRITE(EN_LOG_ERROR, "Fan Board Ptr is nullptr.");
        return false;
    }

    if (false == m_stpFanBoardPtr->SetFanLedState(m_uiFanBoardIdx, ucLedState)) {
        LOG_WRITE(EN_LOG_ERROR, "Set Fan(%d) Led State(%d) error.", m_uiFanBoardIdx, ucLedState);
        return false;
    }

    return true;
}
#endif

bool
CFan::SetFanInitFanController()
{
    if (nullptr == m_stpMax31760Ptr) {
        LOG_WRITE(EN_LOG_ERROR, "Fan Board Ptr is nullptr.");
        return false;
    }

    if (false == m_stpMax31760Ptr->InitFanController()) {
        LOG_WRITE(EN_LOG_ERROR, "Set Fan(%d) SetFanInitFanController error.", m_uiFanBoardIdx);
        return false;
    }

    return true;
}

bool
CFan::SetFanControllerEEP()
{
    if (nullptr == m_stpMax31760Ptr) {
        LOG_WRITE(EN_LOG_ERROR, "Fan Board Ptr is nullptr.");
        return false;
    }

    if (false == m_stpMax31760Ptr->SetFanController()) {
        LOG_WRITE(EN_LOG_ERROR, "Set Fan(%d) SetFanControllerEEP error.", m_uiFanBoardIdx);
        return false;
    }

    return true;
}

