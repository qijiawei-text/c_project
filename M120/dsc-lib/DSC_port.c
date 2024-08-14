
#include "DSC_edfa.h"
#include "DSC_port.h"
#include "DSC_tca.h"
#include "DSC_waveplan.h"
#include "DSC_common.h"

static PORT_INFO_ST m_astPortInfo[MAX_PORT] = {{0}};
static int m_iPortCount = 0;

static PORT_WAVELENGTH_TYPE_EN
__portWavelengthTypeToEnum(char *pcStr)
{
    if (0 == strncmp(pcStr, PORT_TYPE_WAVELENTH_STR, strlen(PORT_TYPE_WAVELENTH_STR))) {
        return PORT_TYPE_WAVELENGTH;
    } else if (0 == strncmp(pcStr, PORT_TYPE_MULTI_WAVELENGTH_STR, strlen(PORT_TYPE_MULTI_WAVELENGTH_STR))) {
        return PORT_TYPE_MULTI_WAVELENGTH;
    } else if (0 == strncmp(pcStr, PORT_TYPE_ALL_WAVELENGTH_STR, strlen(PORT_TYPE_ALL_WAVELENGTH_STR))) {
        return PORT_TYPE_ALL_WAVELENGTH;
    }
    return PORT_TYPE_MULTI_WAVELENGTH;
}

static PORT_QUAL_TYPE_EN
__portQualTypeToEnum(char *pcStr)
{

    if (0 == strncmp(pcStr, PORT_QUAL_TYPE_INTERNAL_STR, strlen(PORT_QUAL_TYPE_INTERNAL_STR))) {
        return PORT_TYPE_ROADM_INTERNAL;
    } else if (0 == strncmp(pcStr, PORT_QUAL_TYPE_EXTERNAL_STR, strlen(PORT_QUAL_TYPE_EXTERNAL_STR))) {
        return PORT_TYPE_ROADM_EXTERNAL;
    } else if (0 == strncmp(pcStr, PORT_QUAL_TYPE_NETWORK_STR, strlen(PORT_QUAL_TYPE_NETWORK_STR))) {
        return PORT_TYPE_XPDR_NETWORK;
    } else if (0 == strncmp(pcStr, PORT_QUAL_TYPE_CLIENT_STR, strlen(PORT_QUAL_TYPE_CLIENT_STR))) {
        return PORT_TYPE_XPDR_CLIENT;
    } else if (0 == strncmp(pcStr, PORT_QUAL_TYPE_OTDR_STR, strlen(PORT_QUAL_TYPE_OTDR_STR))) {
        return PORT_TYPE_OTDR;
    } else if (0 == strncmp(pcStr, PORT_QUAL_TYPE_NETWORK_BK_STR, strlen(PORT_QUAL_TYPE_NETWORK_BK_STR))) {
        return PORT_TYPE_XPDR_NETWORK_BK;
    }

    return PORT_TYPE_ROADM_INTERNAL;
}

PORT_WAVELENGTH_TYPE_EN
DSC_PortWavelengthTypeToEnum(char *pcStr)
{
    return __portWavelengthTypeToEnum(pcStr);
}

PORT_QUAL_TYPE_EN
DSC_PortQualTypeToEnum(char *pcStr)
{
    return __portQualTypeToEnum(pcStr);
}

DIRECTION_EN
DSC_PortDirectionToEnum(char *pcStr)
{
    if (0 == strncmp(pcStr, PORT_DIRECTION_RX_STR, strlen(PORT_DIRECTION_RX_STR))) {
        return DIRECTION_RX;
    } else if (0 == strncmp(pcStr, PORT_DIRECTION_TX_STR, strlen(PORT_DIRECTION_TX_STR))) {
        return DIRECTION_TX;
    }

    return DIRECTION_RX;
}



char *
DSC_DirectionToString(DIRECTION_EN enDirection, char *pcBuf)
{
    if (NULL == pcBuf) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        return NULL;
    }

    switch (enDirection) {
    case DIRECTION_RX:
        strcpy(pcBuf, PORT_DIRECTION_RX_STR);
        break;
    case DIRECTION_TX:
        strcpy(pcBuf, PORT_DIRECTION_TX_STR);
        break;
    default:
        strcpy(pcBuf, "N/A");
        break;
    }

    return pcBuf;
}

char *
DSC_PortQualTypeToString(PORT_QUAL_TYPE_EN enQualType, char *pcBuf)
{
    if (NULL == pcBuf) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        return NULL;
    }

    switch (enQualType) {
    case PORT_TYPE_ROADM_INTERNAL:
        strcpy(pcBuf, PORT_QUAL_TYPE_INTERNAL_STR);
        break;
    case PORT_TYPE_ROADM_EXTERNAL:
        strcpy(pcBuf, PORT_QUAL_TYPE_EXTERNAL_STR);
        break;
    case PORT_TYPE_OTDR:
        strcpy(pcBuf, PORT_QUAL_TYPE_OTDR_STR);
        break;
    case PORT_TYPE_XPDR_NETWORK:
        strcpy(pcBuf, PORT_QUAL_TYPE_NETWORK_STR);
        break;
    case PORT_TYPE_XPDR_CLIENT:
        strcpy(pcBuf, PORT_QUAL_TYPE_CLIENT_STR);
        break;
    case PORT_TYPE_XPDR_NETWORK_BK:
        strcpy(pcBuf, PORT_QUAL_TYPE_NETWORK_BK_STR);
        break;
    default:
        break;
    }

    return pcBuf;
}

char *
DSC_PortWavelengthTypeToString(PORT_WAVELENGTH_TYPE_EN enQualType, char *pcBuf)
{
    if (NULL == pcBuf) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        return NULL;
    }

    switch (enQualType) {
    case PORT_TYPE_WAVELENGTH:
        strcpy(pcBuf, PORT_TYPE_WAVELENTH_STR);
        break;
    case PORT_TYPE_MULTI_WAVELENGTH:
        strcpy(pcBuf, PORT_TYPE_MULTI_WAVELENGTH_STR);
        break;
    case PORT_TYPE_ALL_WAVELENGTH:
        strcpy(pcBuf, PORT_TYPE_ALL_WAVELENGTH_STR);
        break;
    default:
        break;
    }

    return pcBuf;
}

bool
DSC_PortExist(uint32_t u32PortId)
{
    return true;
}

int
DSC_GetPortNumber(char *xpath)
{
    sr_xpath_ctx_t state = {0};
    char *key = NULL;
    int port_id = -1;

    key = sr_xpath_key_value(xpath, "ports", "port-id", &state);
    if (key != NULL) {
        port_id = atoi(key);
    }

    sr_xpath_recover(&state);
    return port_id;
}

int
DSC_GetChannelNumber(char *xpath)
{
    sr_xpath_ctx_t state = {0};
    char *key = NULL;
    int chan_id = -1;

    key = sr_xpath_key_value(xpath, "used-wavelengths", "wavelength-number", &state);
    if (key != NULL) {
        chan_id = atoi(key);
    }

    sr_xpath_recover(&state);
    return chan_id;
}


int
DSC_PortDirection(uint32_t u32PortId, DIRECTION_EN *penDirection)
{
#if 1
    size_t count = 0;
    PORT_INFO_ST *pstPorts = NULL;
    static bool bReadFlag = false;

    int i = u32PortId - 1;
    if (i >= MAX_PORT) {
        return SR_ERR_INTERNAL;
    }

    if (!bReadFlag) {
        DSC_GetPortsInfo(&pstPorts, &count);
        bReadFlag = true;
    }

    *penDirection = m_astPortInfo[i].enPortDirection;
    return 0;
#else
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    char acBuf[16];
    char xpath[OPLK_PATH_MAX_LEN];

    sess = DSC_GetOperSession();
    if (NULL == sess || NULL == penDirection) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    memset(xpath, 0, sizeof(xpath));
    sprintf(xpath, PORT_XPATH_DRIECTION, u32PortId);
    rc = get_enum_item(sess, xpath, acBuf, sizeof(acBuf));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get_enum_item: %s", xpath);
        return rc;
    }

    *penDirection = DSC_PortDirectionToEnum(acBuf);
    return rc;
#endif
}

int
DSC_PortEdfaDirection(uint32_t u32PortId, DIRECTION_EN *penDirection)
{
#if 1
    size_t count = 0;
    PORT_INFO_ST *pstPorts = NULL;
    static bool bReadFlag = false;

    int i = u32PortId - 1;
    if (i >= MAX_PORT) {
        return SR_ERR_INTERNAL;
    }

    if (!bReadFlag) {
        DSC_GetPortsInfo(&pstPorts, &count);
        bReadFlag = true;
    }

    *penDirection = m_astPortInfo[i].enEdfaDirection;
    return 0;

#else
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    char xpath[OPLK_PATH_MAX_LEN];
    uint32_t u32EdfaId = 0;

    sess = DSC_GetOperSession();
    if (NULL == sess || NULL == penDirection) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    memset(xpath, 0, sizeof(xpath));
    sprintf(xpath, PORT_XPATH_EDFA_ID, u32PortId);
    rc = get_uint32_item(sess, xpath, &u32EdfaId);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get_uint32_item: %s", xpath);
    }

    *penDirection = u32EdfaId;

    return rc;
#endif
}

int
DSC_PortQualType(uint32_t u32PortId, PORT_QUAL_TYPE_EN *penType)
{
#if 1
    size_t count = 0;
    PORT_INFO_ST *pstPorts = NULL;
    static bool bReadFlag = false;

    int i = u32PortId - 1;
    if (i >= MAX_PORT) {
        return SR_ERR_INTERNAL;
    }

    if (!bReadFlag) {
        DSC_GetPortsInfo(&pstPorts, &count);
        bReadFlag = true;
    }

    *penType = m_astPortInfo[i].enPortQualType;
    return 0;

#else
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    char acBuf[16];
    char xpath[OPLK_PATH_MAX_LEN];

    sess = DSC_GetOperSession();
    if (NULL == sess || NULL == penType) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    memset(xpath, 0, sizeof(xpath));
    sprintf(xpath, PORT_XPATH_QUAL_TYPE, u32PortId);
    rc = get_enum_item(sess, xpath, acBuf, sizeof(acBuf));
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get_enum_item: %s", xpath);
        return rc;
    }

    *penType = __portQualTypeToEnum(acBuf);

    return rc;
#endif
}


int
DSC_GetPortsCnt(size_t *count)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    sr_val_t *values = NULL;
    sess = DSC_GetOperSession();

    if (NULL == sess || NULL == count) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        return SR_ERR_INVAL_ARG;
    }

    /* the number of ports never changes, so cache it */
    if (m_iPortCount > 0 ) {
        *count = m_iPortCount;
        return rc;
    }

    rc = get_items(sess, PORT_XPATH_ID_ALL, &values, count);
    if (rc != SR_ERR_OK) {
        LOG_WRITE(EN_LOG_ERROR, "get_items: %s", PORT_XPATH_ID_ALL);
    }

    /* save it */
    m_iPortCount = *count;
    sr_free_values(values, *count);

    return rc;
}

#if 0
int
DSC_GetPortsInfo_Iter(PORT_INFO_ST **ppstPortInfo, size_t *count)
{
    int rc = SR_ERR_OK;
    int i = 0;
    sr_session_ctx_t *sess = NULL;
    sr_val_iter_t *iter = NULL;
    sr_val_t *value = NULL;
    char selectXpath[OPLK_PATH_MAX_LEN];
    PORT_INFO_ST *port = (PORT_INFO_ST *)&m_astPortInfo;
    static bool bReadFlag = false;

    sess = DSC_GetOperSession();
    if (NULL == sess || NULL == count) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        goto ERR_LABEL;
    }

    if (!bReadFlag) {
        memset(port, 0, sizeof(m_astPortInfo));
        memset(selectXpath, 0, sizeof(selectXpath));
        snprintf(selectXpath, OPLK_PATH_MAX_LEN, PORT_XPATH_ALL);
        rc = sr_get_items_iter(sess, selectXpath, &iter);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by sr_get_items_iter: %s", sr_strerror(rc));
            goto ERR_LABEL;
        }

        i = -1;
        while (SR_ERR_OK == sr_get_item_next(sess, iter, &value)) {
            /*
             * <ports>
             *     <port-id>1</port-id>
             *     <port>
             *         <port-id>1</port-id>
             *         <port-name>InPort</port-name>
             *     </port>
             * </ports>
             */
            /*
             * For there are two 'port-id', so we need to seperate them
             * /org-openroadm-device:org-openroadm-device/ports[port-id='1']/port-id  is Right
             * /org-openroadm-device:org-openroadm-device/ports[port-id='1']/port/port-id
             */
            if (strstr(value->xpath, "]"PORT_ID) != NULL) {
                i++;
                port[i].u32PortId = value->data.uint32_val;
            } else if (strstr(value->xpath, PORT_NAME) != NULL) {
                strcpy(port[i].acPortName, value->data.string_val);
            } else if (strstr(value->xpath, PORT_DIRECTION) != NULL) {
                port[i].enPortDirection = DSC_PortDirectionToEnum(value->data.enum_val);
            } else if (strstr(value->xpath, PORT_QUAL_TYPE) != NULL) {
                port[i].enPortQualType = __portQualTypeToEnum(value->data.enum_val);
            } else if (strstr(value->xpath, TARGET_POWER_PORT_XPATH) != NULL) {
                port[i].dTargetPortPower = value->data.decimal64_val;
            } else if (strstr(value->xpath, PORT_EDFA_ID) != NULL) {
                port[i].enEdfaDirection = value->data.uint32_val;
            } else if (strstr(value->xpath, PORT_OSC_ID) != NULL) {
                port[i].enOscDirection = value->data.uint32_val;
            } else if (strstr(value->xpath, PORT_CAPA_MAX_RX) != NULL) {
                port[i].stPortProperty.dCapabilityMaxRxPower = value->data.decimal64_val;
            } else if (strstr(value->xpath, PORT_CAPA_MIN_RX) != NULL) {
                port[i].stPortProperty.dCapabilityMinRxPower = value->data.decimal64_val;
            } else if (strstr(value->xpath, PORT_CAPA_MAX_TX) != NULL) {
                port[i].stPortProperty.dCapabilityMaxTxPower = value->data.decimal64_val;
            } else if (strstr(value->xpath, PORT_CAPA_MIN_TX) != NULL) {
                port[i].stPortProperty.dCapabilityMinTxPower = value->data.decimal64_val;
            }
            sr_free_val(value);
        }
        sr_free_val_iter(iter);

        m_iPortCount = i + 1;
        bReadFlag = true;
    }

    *ppstPortInfo = port;
    *count = m_iPortCount;

    return rc;

ERR_LABEL:
    if (iter != NULL)
        sr_free_val_iter(iter);
    if (value != NULL)
        sr_free_val(value);
    *count = 0;
    return rc;
}
#endif

int
DSC_GetPortsInfo(PORT_INFO_ST **ppstPortInfo, size_t *count)
{
    int rc = SR_ERR_OK;
    int i = 0, j = 0;
    sr_session_ctx_t *sess = NULL;
    sr_val_t *val = NULL;
    size_t cnt = 0;
    char selectXpath[OPLK_PATH_MAX_LEN];
    PORT_INFO_ST *port = (PORT_INFO_ST *)&m_astPortInfo;
    static bool bReadFlag = false;

    sess = DSC_GetOperSession();
    if (NULL == sess || NULL == count) {
        LOG_WRITE(EN_LOG_ERROR, "input null %d", SR_ERR_INVAL_ARG);
        goto ERR_LABEL;
    }

    if (!bReadFlag) {
        memset(port, 0, sizeof(m_astPortInfo));
        memset(selectXpath, 0, sizeof(selectXpath));
        snprintf(selectXpath, OPLK_PATH_MAX_LEN, PORT_XPATH_ALL);
        rc = get_items(sess, selectXpath, &val, &cnt);
        if (SR_ERR_NOT_FOUND == rc) {
            LOG_WRITE(EN_LOG_DEBUG, "Not found item: %s, xpath: %s", sr_strerror(rc), selectXpath);
            goto ERR_LABEL;
        }

        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Error by get_items: %s", sr_strerror(rc));
            goto ERR_LABEL;
        }

        j = -1;
        for (i = 0; i < cnt; i++) {
            /*
             * <ports>
             *     <port-id>1</port-id>
             *     <port>
             *         <port-id>1</port-id>
             *         <port-name>InPort</port-name>
             *     </port>
             * </ports>
             */
            /*
             * For there are two 'port-id', so we need to seperate them
             * /org-openroadm-device:org-openroadm-device/ports[port-id='1']/port-id  is Right
             * /org-openroadm-device:org-openroadm-device/ports[port-id='1']/port/port-id
             */
            if (strstr(val[i].xpath, "]"PORT_ID) != NULL) {
                j++;
                port[j].u32PortId = val[i].data.uint32_val;
            } else if (strstr(val[i].xpath, PORT_NAME) != NULL) {
                strcpy(port[j].acPortName, val[i].data.string_val);
            } else if (strstr(val[i].xpath, PORT_DIRECTION) != NULL) {
                port[j].enPortDirection = DSC_PortDirectionToEnum(val[i].data.enum_val);
            } else if (strstr(val[i].xpath, PORT_QUAL_TYPE) != NULL) {
                port[j].enPortQualType = __portQualTypeToEnum(val[i].data.enum_val);
            } else if (strstr(val[i].xpath, TARGET_POWER_PORT_XPATH) != NULL) {
                port[j].dTargetPortPower = val[i].data.decimal64_val;
            } else if (strstr(val[i].xpath, PORT_EDFA_ID) != NULL) {
                port[j].enEdfaDirection = val[i].data.uint32_val;
            } else if (strstr(val[i].xpath, PORT_OSC_ID) != NULL) {
                port[j].enOscDirection = val[i].data.uint32_val;
            } else if (strstr(val[i].xpath, PORT_CAPA_MAX_RX) != NULL) {
                port[j].stPortProperty.dCapabilityMaxRxPower = val[i].data.decimal64_val;
            } else if (strstr(val[i].xpath, PORT_CAPA_MIN_RX) != NULL) {
                port[j].stPortProperty.dCapabilityMinRxPower = val[i].data.decimal64_val;
            } else if (strstr(val[i].xpath, PORT_CAPA_MAX_TX) != NULL) {
                port[j].stPortProperty.dCapabilityMaxTxPower = val[i].data.decimal64_val;
            } else if (strstr(val[i].xpath, PORT_CAPA_MIN_TX) != NULL) {
                port[j].stPortProperty.dCapabilityMinTxPower = val[i].data.decimal64_val;
            }
        }
        sr_free_values(val, cnt);

        m_iPortCount = j + 1;
        bReadFlag = true;
    }

    *ppstPortInfo = port;
    *count = m_iPortCount;

    return rc;

ERR_LABEL:
    if (val != NULL)
        sr_free_values(val, cnt);
    *count = 0;
    return rc;
}

int
DSC_GetPortInfo(uint32_t u32PortId, PORT_INFO_ST *pstPortInfo, size_t size)
{
    int rc = SR_ERR_OK;
    sr_session_ctx_t *sess = NULL;
    char selectXpath[OPLK_PATH_MAX_LEN];
    char xpath[16][OPLK_PATH_MAX_LEN];
    char aacEnum[4][32];

    memset(xpath, 0, sizeof(xpath));
    memset(aacEnum, 0, sizeof(aacEnum));

    ANONYMOUS_ST port_maps[] = {
        ANONYMOUS_MAP_ENTRY(__sprintfInt(xpath[0], PORT_XPATH_NAME, u32PortId), SR_STRING_T, pstPortInfo->acPortName),
        ANONYMOUS_MAP_ENTRY(__sprintfInt(xpath[1], PORT_XPATH_DRIECTION, u32PortId), SR_ENUM_T, aacEnum[0]),
        ANONYMOUS_MAP_ENTRY(__sprintfInt(xpath[2], PORT_XPATH_TYPE, u32PortId), SR_ENUM_T, aacEnum[1]),
        ANONYMOUS_MAP_ENTRY(__sprintfInt(xpath[3], PORT_XPATH_QUAL_TYPE, u32PortId), SR_ENUM_T, aacEnum[2]),
        ANONYMOUS_MAP_ENTRY(__sprintfInt(xpath[4], PORT_XPATH_CURRENT_POWER, u32PortId), SR_DECIMAL64_T, (char *)(&pstPortInfo->dCurrentPortPower)),
        ANONYMOUS_MAP_ENTRY(__sprintfInt(xpath[5], PORT_XPATH_EDFA_ID, u32PortId), SR_UINT32_T, (char *)(&pstPortInfo->enEdfaDirection)),
        ANONYMOUS_MAP_ENTRY(__sprintfInt(xpath[6], PORT_XPATH_OSC_ID, u32PortId), SR_UINT32_T, (char *)(&pstPortInfo->enOscDirection)),
        ANONYMOUS_MAP_ENTRY(__sprintfInt(xpath[7], PORT_XPATH_CAPA_MAX_RX, u32PortId), SR_DECIMAL64_T, (char *)(&pstPortInfo->stPortProperty.dCapabilityMaxRxPower)),
        ANONYMOUS_MAP_ENTRY(__sprintfInt(xpath[8], PORT_XPATH_CAPA_MIN_RX, u32PortId), SR_DECIMAL64_T, (char *)(&pstPortInfo->stPortProperty.dCapabilityMinRxPower)),
        ANONYMOUS_MAP_ENTRY(__sprintfInt(xpath[9], PORT_XPATH_CAPA_MAX_TX, u32PortId), SR_DECIMAL64_T, (char *)(&pstPortInfo->stPortProperty.dCapabilityMaxTxPower)),
        ANONYMOUS_MAP_ENTRY(__sprintfInt(xpath[10], PORT_XPATH_CAPA_MIN_TX, u32PortId), SR_DECIMAL64_T, (char *)(&pstPortInfo->stPortProperty.dCapabilityMinTxPower)),
    };

    sess = DSC_GetOperSession();

    if (NULL == sess || NULL == pstPortInfo || size < sizeof(PORT_INFO_ST)) {
        LOG_WRITE(EN_LOG_ERROR, "input null, or invalid length:%zu", size);
        return SR_ERR_INVAL_ARG;
    }

    memset((void *)pstPortInfo, 0, size);
    pstPortInfo->u32PortId = u32PortId;

    memset(selectXpath, 0, sizeof(selectXpath));
    sprintf(selectXpath, PORT_XPATH_SINGLE, u32PortId);
    rc = DSC_GetItems(sess, selectXpath, port_maps, sizeof(port_maps));
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetItems: %s", sr_strerror(rc));
        return rc;
    }

    pstPortInfo->enPortDirection = DSC_PortDirectionToEnum(aacEnum[0]);
    pstPortInfo->enPortWavelengthType = __portWavelengthTypeToEnum(aacEnum[1]);
    pstPortInfo->enPortQualType = __portQualTypeToEnum(aacEnum[2]);

    /* Get thresh from TCA */
    {
        int rc = SR_ERR_OK;
        TCA_INFO_ST *pstTcaInfo = NULL;
        size_t count = 0;

        rc = DSC_GetTcaList(NULL, &pstTcaInfo, &count);
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "Get tca list error");
            return rc;
        }

        DSC_GetTcaThresh((TCA_IDX_PORT_LINE_IN + (u32PortId - PORT_ID_LINE_IN)),
                         pstTcaInfo,
                         &(pstPortInfo->dHighThr),
                         &(pstPortInfo->dLowThr));
    }
    return rc;
}


int
DSC_GetPortCurrentPower(uint32_t u32PortId, double *pdCurrentPower)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetOperSession();
    if ((NULL == sess) || (NULL == pdCurrentPower)) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        goto ERR_LABEL;
    }

    *pdCurrentPower = NO_POWER;

    memset(acSelectXpath, 0, sizeof(acSelectXpath));
    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, PORT_XPATH_CURRENT_POWER, u32PortId);
    rc = get_decimal64_item(sess, acSelectXpath, pdCurrentPower);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get_decimal64_item: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

ERR_LABEL:

    if (SR_ERR_NOT_FOUND == rc) {
        rc = SR_ERR_OK;
    }
    return rc;
}

int
DSC_SetPortTargetPower(uint32_t u32PortId, double dTargetPortPower)
{
    int rc = SR_ERR_OK;
    char acSelectXpath[OPLK_PATH_MAX_LEN];
    DIRECTION_EN enDirection = 0;
    size_t u32PortCnt = 0;


    sr_session_ctx_t *sess = DSC_GetRunningSession();
    if (NULL == sess) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "input null");
        goto ERR_LABEL;
    }

    rc = DSC_GetPortsCnt(&u32PortCnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    if ((u32PortId > u32PortCnt) || (!DSC_PortExist(u32PortId))) {
        rc = SR_ERR_INTERNAL;
        LOG_WRITE(EN_LOG_ERROR, "Port %d is overflow or not exist", u32PortId);
        goto ERR_LABEL;
    }

    rc = DSC_PortDirection(u32PortId, &enDirection);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_PortDirection: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    if (DIRECTION_RX == enDirection) {
        rc = SR_ERR_INVAL_ARG;
        LOG_WRITE(EN_LOG_ERROR, "Port %d is inport, Can not config target power", u32PortId);
        goto ERR_LABEL;
    }

    snprintf(acSelectXpath, OPLK_PATH_MAX_LEN, PORT_XPATH_TARGET_POWER, u32PortId);
    rc = set_decimal64_item(sess, acSelectXpath, dTargetPortPower);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}

int
DSC_GetPortUsedChannelsCnt(uint32_t u32PortId, size_t *count)
{
    int rc = SR_ERR_OK;

    return rc;
}

#if 0
int
DSC_GetPortUsedChannelsInfo_Iter(uint32_t u32PortId, USED_WAVELENGTHS_ST **ppstChannelInfo, size_t *count)
{
    int rc = SR_ERR_OK;
    int i = 0;
    sr_session_ctx_t *sess = NULL;
    sr_val_t *value = NULL;
    sr_val_iter_t *iter = NULL;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    USED_WAVELENGTHS_ST *pstInfo = NULL;
    size_t tMaxWaveCnt = 0;

    sess = DSC_GetOperSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        return SR_ERR_INVAL_ARG;
    }

    rc = DSC_GetDefaultWaveplansCnt(&tMaxWaveCnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get default waveplan cnt %s", sr_strerror(rc));
        return SR_ERR_INVAL_ARG;
    }

    if (0 == tMaxWaveCnt) {
        return SR_ERR_OK;
    } else if (tMaxWaveCnt >= MAX_CHL) {
        LOG_WRITE(EN_LOG_ERROR, "Wavelen number: %d out of range", tMaxWaveCnt);
        return SR_ERR_OPERATION_FAILED;
    }

    pstInfo = malloc(sizeof(USED_WAVELENGTHS_ST) * tMaxWaveCnt);
    if (NULL == pstInfo) {
        LOG_WRITE(EN_LOG_ERROR, "malloc error %s", sr_strerror(rc));
        return SR_ERR_OPERATION_FAILED;
    }

    memset(pstInfo, 0, sizeof(USED_WAVELENGTHS_ST) * tMaxWaveCnt);
    for (i = 0; i < tMaxWaveCnt; i++) {
        pstInfo[i].dCurrentChannelPower = NO_POWER;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, CHANNEL_XPATH_ALL, u32PortId);
    rc = sr_get_items_iter(sess, acXpath, &iter);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_get_items_iter: %s", sr_strerror(rc));
        return rc;
    }

    i = -1;
    while (SR_ERR_OK == sr_get_item_next(sess, iter, &value)) {
        /*
           /org-openroadm-device:org-openroadm-device/ports[port-id='3']/port/used-wavelengths[wavelength-number='84']/wavelength-number
           /org-openroadm-device:org-openroadm-device/ports[port-id='3']/port/used-wavelengths[wavelength-number='84']/wavelength-current-power
         */
        if ((int)u32PortId != DSC_GetPortNumber(value->xpath)) {
            sr_free_val(value);
            continue;
        }

        if (strstr(value->xpath, "]" CHANNEL_WL_NUMBER) != NULL) {
            i++;
            if (i >= tMaxWaveCnt) {
                break;
            }
            pstInfo[i].u32ChannelId = value->data.uint32_val;
        } else if (NULL != strstr(value->xpath, CHANNEL_WL_CURRENT_POWER)) {
            if (i >= 0 && i < tMaxWaveCnt) {
                pstInfo[i].dCurrentChannelPower = value->data.decimal64_val;
            }
        } else {
        }

        sr_free_val(value);
    }
    sr_free_val_iter(iter);

    *ppstChannelInfo = pstInfo;
    *count = ((i + 1) < tMaxWaveCnt ? (i + 1) : tMaxWaveCnt);

    return rc;
}
#endif

int
DSC_GetPortUsedChannelsInfo(uint32_t u32PortId, USED_WAVELENGTHS_ST **ppstChannelInfo, size_t *count)
{
    int rc = SR_ERR_OK;
    int i = 0, j = 0;
    sr_session_ctx_t *sess = NULL;
    sr_val_t *val = NULL;
    size_t cnt = 0;
    size_t tMaxWaveCnt = 0;
    char acXpath[OPLK_PATH_MAX_LEN] = {0};
    USED_WAVELENGTHS_ST *pstInfo = NULL;

    if (NULL == count) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "count");
        return SR_ERR_INVAL_ARG;
    }

    sess = DSC_GetOperSession();
    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "input null %s", "sess");
        return SR_ERR_INVAL_ARG;
    }

    memset(acXpath, 0x00, sizeof(acXpath));
    snprintf(acXpath, OPLK_PATH_MAX_LEN, CHANNEL_XPATH_ALL, u32PortId);
    rc = get_items(sess, acXpath, &val, &cnt);
    if (SR_ERR_OK != rc) {
        if (SR_ERR_NOT_FOUND == rc) {
            *count = 0;
            return SR_ERR_OK;
        } else {
            LOG_WRITE(EN_LOG_ERROR, "Error by get_items %s, %s", acXpath, sr_strerror(rc));
            return rc;
        }
    }

    rc = DSC_GetOcmWaveplanNumber(&tMaxWaveCnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by get default waveplan cnt %s", sr_strerror(rc));
        return SR_ERR_INVAL_ARG;
    }

    if (0 == tMaxWaveCnt) {
        *count = 0;
        return SR_ERR_OK;
    } else if (tMaxWaveCnt >= MAX_CHL) {
        LOG_WRITE(EN_LOG_ERROR, "Default waveplan count: %zu error", tMaxWaveCnt);
        return SR_ERR_INVAL_ARG;
    }

    pstInfo = malloc(sizeof(USED_WAVELENGTHS_ST) * tMaxWaveCnt);
    if (NULL == pstInfo) {
        LOG_WRITE(EN_LOG_ERROR, "malloc error %s", "");
        return SR_ERR_OPERATION_FAILED;
    }
    memset(pstInfo, 0, sizeof(USED_WAVELENGTHS_ST) * tMaxWaveCnt);

    j = -1;
    for (i = 0; i < cnt; i++) {
        /*
           /org-openroadm-device:org-openroadm-device/ports[port-id='1']/port/used-wavelengths[wavelength-number='78']/wavelength-number
         */
        if ((int)u32PortId != DSC_GetPortNumber(val[i].xpath)) {
            continue;
        }

        if (strstr(val[i].xpath, "]" CHANNEL_WL_NUMBER) != NULL) {
            j++;
            if (j >= tMaxWaveCnt) {
                break;
            }
            pstInfo[j].u32ChannelId = val[i].data.uint32_val;
        } else if (NULL != strstr(val[i].xpath, CHANNEL_WL_CURRENT_POWER)) {
            if (j >= 0 && j < tMaxWaveCnt) {
                pstInfo[j].dCurrentChannelPower = val[i].data.decimal64_val;
            }
        } else {
        }
    }

    sr_free_values(val, cnt);

    *ppstChannelInfo = pstInfo;
    *count = ((j + 1) < tMaxWaveCnt ? (j + 1) : tMaxWaveCnt);

    return rc;
}


int
DSC_SetChannelTargetPower(uint32_t u32PortId, uint32_t u32ChannelId, double dTargetPower)
{
    int rc = SR_ERR_OK;
    char selectXpath[OPLK_PATH_MAX_LEN];

    sr_session_ctx_t *sess = DSC_GetOperSession();

    if (NULL == sess) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s\n", sr_strerror(rc));
        goto ERR_LABEL;
    }

    snprintf(selectXpath, OPLK_PATH_MAX_LEN, CHANNEL_XPATH_TARGET_POWER, u32PortId, u32ChannelId);

    rc = set_decimal64_item(sess, selectXpath, dTargetPower);

    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by sr_set: %s\n", sr_strerror(rc));
        goto ERR_LABEL;
    }

    rc = DSC_Commit(sess);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_Commit: %s\n", sr_strerror(rc));
    }

ERR_LABEL:
    return rc;
}


int
DSC_SetPortThreshold(uint32_t u32PortId, double dHighThr, double dLowThr)
{
    int rc = SR_ERR_OK;
    EN_TCA_IDX enTcaIdx = 0;

    if (0 == u32PortId) {
        LOG_WRITE(EN_LOG_ERROR, "Error input port: %u", u32PortId);
        return SR_ERR_INVAL_ARG;
    } else {
        enTcaIdx = TCA_IDX_PORT_LINE_IN + (u32PortId - PORT_ID_LINE_IN);
    }
    rc = DSC_SetTcaThreshold(NULL, enTcaIdx, dHighThr, dLowThr);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error set port:%u thresh: %s", u32PortId, sr_strerror(rc));
    }

    return rc;
}

int
DSC_GetPortIdByName(char *port_name, unsigned int *id)
{
    int rc = SR_ERR_OK, i = 0;
    size_t cnt = 0;
    sr_session_ctx_t *sess = NULL;
    char selectXpath[OPLK_PATH_MAX_LEN];
    char acTmp[64] = {0};

    sess = DSC_GetOperSession();
    rc = DSC_GetPortsCnt(&cnt);
    if (SR_ERR_OK != rc) {
        LOG_WRITE(EN_LOG_ERROR, "Error by DSC_GetPortsCnt: %s", sr_strerror(rc));
        return rc;
    }

    for (i = 1; i <= cnt; i++) {
        snprintf(selectXpath, OPLK_PATH_MAX_LEN, PORT_XPATH_NAME, i);
        memset(acTmp, 0, sizeof(acTmp));
        rc = get_string_item(sess, selectXpath, acTmp, sizeof(acTmp));
        if (SR_ERR_OK != rc) {
            LOG_WRITE(EN_LOG_ERROR, "get_string_item: %s %s", selectXpath, sr_strerror(rc));
            return rc;
        }

        if (strcmp(acTmp, port_name) == 0) {
            *id = i;
        }
    }

    return rc;
}

