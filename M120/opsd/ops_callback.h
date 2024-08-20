#ifndef __OPS_CALLBACK_H__
#define __OPS_CALLBACK_H__

#ifdef __cplusplus
extern "C" {
#endif

#define MIN_OPS_PWR_DIFF_THR_DELTA 50


typedef struct OPS_RANGE_st {
    uint32_t uiWtrMax;
    uint32_t uiWtrMin;
    uint32_t uiHoldOffMax;
    uint32_t uiHoldOffMin;
    double dPrimaryThrMax;
    double dPrimaryThrMin;
    double dPrimaryHysMax;
    double dPrimaryHysMin;
    double dSecondaryThrMax;
    double dSecondaryThrMin;
    double dSecondaryHysMax;
    double dSecondaryHysMin;
    double dRelativeThrMax;
    double dRelativeThrMin;
    double dRelativeOffsetMax;
    double dRelativeOffsetMin;
    double dComRLosThrMax;
    double dComRLosThrMin;
    double dComRLosHysMax;
    double dComRLosHysMin;
    double dDifferThrMax;
    double dDifferThrMin;
    double dDifferHysMax;
    double dDifferHysMin;
    double dComRLowThrMax;
    double dComRLowThrMin;
    double dComRLowHysMax;
    double dComRLowHysMin;
    double dComRHighThrMax;
    double dComRHighThrMin;
    double dComRHighHysMax;
    double dComRHighHysMin;
    double dPriRLowThrMax;
    double dPriRLowThrMin;
    double dPriRLowHysMax;
    double dPriRLowHysMin;
    double dPriRHighThrMax;
    double dPriRHighThrMin;
    double dPriRHighHysMax;
    double dPriRHighHysMin;
    double dSecRLowThrMax;
    double dSecRLowThrMin;
    double dSecRLowHysMax;
    double dSecRLowHysMin;
    double dSecRHighThrMax;
    double dSecRHighThrMin;
    double dSecRHighHysMax;
    double dSecRHighHysMin;
} OPS_RANGE_ST;

typedef struct {
    bool bRevertive;
    bool bWtr;
    bool bHoldOffTime;
    bool bPrimaryThr;
    bool bPrimaryHys;
    bool bSecondaryThr;
    bool bSecondaryHys;
    bool bRelativeThr;
    bool bRelativeOffset;
    bool bForceToPort;
} OPS_CFG_CHANGE_FLAG;


/* extern int OPS_OpsCmdStrToDevCmd(char *pcStrCmd, char *pcPath, uint8_t *pucMode); */
int OPS_OpsGetParaRange(OPS_RANGE_ST *pstRange);
int OPS_InitSetMode(uint8_t ucIdx);
int OPS_OpsDataProviderCb(sr_session_ctx_t *session, uint32_t sub_id, const char *module_name, const char *xpath_in,
                          const char *request_xpath, uint32_t request_id, struct lyd_node **parent, void *private_data);
int OPS_RpcOpsCb(sr_session_ctx_t *session, uint32_t sub_id, const char *xpath, const sr_val_t *input, const size_t input_cnt,
                 sr_event_t event, uint32_t request_id, sr_val_t **output, size_t *output_cnt, void *private_data);
int OPS_OpsChangeCb(sr_session_ctx_t *session, uint32_t sub_id, const char *module_name, const char *xpath, sr_event_t event, uint32_t request_id, void *private_ctx);
int OPS_OpsDevEventCb(int iEventId, int iSubEventId);
int OPS_TcaChangeCb(sr_session_ctx_t *session, uint32_t sub_id, const char *module_name, const char *xpath, sr_event_t event, uint32_t request_id, void *private_data);
void OPS_OpsAlmInit(sr_session_ctx_t *pstStartupSess, sr_session_ctx_t *pstRunningSess, bool bInitAlmTable);
void OPS_OpsAlmFree(void);
void OPS_OpsAlmProtoFail(bool bAlmSta);

#ifdef __cplusplus
}
#endif
#endif

