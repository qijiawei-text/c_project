#ifndef _DSC_PORT_H
#define _DSC_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DSC_common.h"

typedef enum en_PORT_WAVELENGTH_TYPE
{
    PORT_TYPE_WAVELENGTH = 1,
    PORT_TYPE_MULTI_WAVELENGTH,
    PORT_TYPE_ALL_WAVELENGTH,
} PORT_WAVELENGTH_TYPE_EN;

typedef enum en_PORT_QUAL_TYPE
{
    PORT_TYPE_ROADM_INTERNAL = 1,
    PORT_TYPE_ROADM_EXTERNAL,
    PORT_TYPE_XPDR_NETWORK,
    PORT_TYPE_XPDR_CLIENT,
    PORT_TYPE_OTDR,
    PORT_TYPE_XPDR_NETWORK_BK
} PORT_QUAL_TYPE_EN;

typedef struct st_PORT_PROPERTY
{
    double dCapabilityMinRxPower;
    double dCapabilityMaxRxPower;
    double dCapabilityMinTxPower;
    double dCapabilityMaxTxPower;
    uint32_t u32CapableWavelengths;
} PORT_PROPERTY_ST;

typedef struct st_USED_WAVELENGTHS
{
    uint32_t u32ChannelId;
    double dTargetChannelPower;
    double dCurrentChannelPower;

} USED_WAVELENGTHS_ST;

typedef struct st_OCM_STATS
{
    uint32_t u32ChannelId;
    double dCurrentPower;
    double dOsnr;
} OCM_STATS_ST;

#define PORT_DIRECTION_RX_STR    "rx"
#define PORT_DIRECTION_TX_STR    "tx"

#define PORT_TYPE_WAVELENTH_STR        "wavelength"
#define PORT_TYPE_MULTI_WAVELENGTH_STR "multi-wavelegnth"
#define PORT_TYPE_ALL_WAVELENGTH_STR   "all-wavelegnth"

#define PORT_QUAL_TYPE_INTERNAL_STR    "roadm-internal"
#define PORT_QUAL_TYPE_EXTERNAL_STR    "roadm-external"
#define PORT_QUAL_TYPE_NETWORK_STR     "network"
#define PORT_QUAL_TYPE_CLIENT_STR      "client"
#define PORT_QUAL_TYPE_OTDR_STR        "otdr"
#define PORT_QUAL_TYPE_NETWORK_BK_STR  "network-backup"

#define PORT_NAME_MUX1_STR             "InC20"
#define PORT_NAME_AUX1IN_STR           "AUX1 IN"
#define PORT_NAME_LINEOUT_STR          "LINE OUT"

#define MAX_PORT           128
/* General port ID */
#define PORT_ID_LINE_IN          (1)
#define PORT_ID_LINE_OUT         (2)

/*Rainier port ID*/
#define RAINIER_PORT_ID_BA_IN      (3)
#define RAINIER_PORT_ID_BA_OUT     (4)
#define RAINIER_PORT_ID_PA_IN      (5)
#define RAINIER_PORT_ID_PA_OUT     (6)

#define PORT_NO_POWER_VALUE        (-9900)
/*
 *  NOPPORO-AMP-OPS port id
 */
typedef enum
{
    EN_NOPPORO_COMBO_OPT_R = 1,
    EN_NOPPORO_COMBO_OPT_T,
    EN_NOPPORO_COMBO_LINE_1_R,
    EN_NOPPORO_COMBO_LINE_1_T,
    EN_NOPPORO_COMBO_LINE_2_R,
    EN_NOPPORO_COMBO_LINE_2_T,
    EN_NOPPORO_COMBO_PORT_ID_CNT,
}EN_NOPPORO_COMBO_PORT_ID;

typedef struct st_PORT_INFO
{
    uint32_t u32PortId;
    char acPortName[32];
    DIRECTION_EN enPortDirection;
    PORT_WAVELENGTH_TYPE_EN enPortWavelengthType;
    PORT_QUAL_TYPE_EN enPortQualType;
    double dTargetPortPower;
    double dCurrentPortPower;
    double dHighThr;
    double dLowThr;
    DIRECTION_EN enEdfaDirection;
    DIRECTION_EN enOscDirection;
    PORT_PROPERTY_ST stPortProperty;
    USED_WAVELENGTHS_ST *pstUsedChannels;
    OCM_STATS_ST *pstChannelsPower;

} PORT_INFO_ST;

/***************************************
* @define port info structure
***************************************/
#define PORT_XPATH_HEADER              OOD_XPATH_PREFIX "/ports"
#define PORT_XPATH                     PORT_XPATH_HEADER "[port-id='%d']"
#define PORTS_XPATH                    PORT_XPATH_HEADER
#define PORT_XPATH_ID_ALL              PORT_XPATH_HEADER "/port-id"
#define PORT_XPATH_ALL                 PORT_XPATH_HEADER "//*"
#define PORT_XPATH_SINGLE              PORT_XPATH "//*"
#define PORT_ID                        "/port-id"
#define PORT_XPATH_ID                  PORT_XPATH PORT_ID
#define PORT_TYPE                      "/port-type"
#define PORT_XPATH_TYPE                PORT_XPATH PORT_TYPE
#define PORT_QUAL_TYPE                 "/port-qual-type"
#define PORT_XPATH_QUAL_TYPE           PORT_XPATH PORT_QUAL_TYPE
#define PORT_DIRECTION                 "/port-direction"
#define PORT_XPATH_DRIECTION           PORT_XPATH PORT_DIRECTION
#define CURRENT_POWER_PORT_XPATH       "/port-current-power"
#define PORT_XPATH_CURRENT_OP_DATA     PORTS_XPATH CURRENT_POWER_PORT_XPATH
#define PORT_XPATH_CURRENT_POWER       PORT_XPATH CURRENT_POWER_PORT_XPATH
#define TARGET_POWER_PORT_XPATH        "/port-target-power"
#define PORT_XPATH_TARGET_POWER        PORT_XPATH TARGET_POWER_PORT_XPATH
#define PORT_NAME                      "/port-name"
#define PORT_XPATH_NAME                PORT_XPATH PORT_NAME
#define PORT_EDFA_ID                   "/edfa-id"
#define PORT_XPATH_EDFA_ID             PORT_XPATH PORT_EDFA_ID
#define PORT_XPATH_EDFA_ID_ALL         PORT_XPATH_HEADER "/port/edfa-id"
#define PORT_OSC_ID                    "/osc-id"
#define PORT_XPATH_OSC_ID              PORT_XPATH PORT_OSC_ID
#define PORT_PROPERTY                  "/port-property"
#define PORT_CAPA_MAX_RX               "/port-power-capability-max-rx"
#define PORT_XPATH_CAPA_MAX_RX         PORT_XPATH "/port-property" PORT_CAPA_MAX_RX
#define PORT_CAPA_MIN_RX               "/port-power-capability-min-rx"
#define PORT_XPATH_CAPA_MIN_RX         PORT_XPATH "/port-property" PORT_CAPA_MIN_RX
#define PORT_CAPA_MAX_TX               "/port-power-capability-max-tx"
#define PORT_XPATH_CAPA_MAX_TX         PORT_XPATH "/port-property" PORT_CAPA_MAX_TX
#define PORT_CAPA_MIN_TX               "/port-power-capability-min-tx"
#define PORT_XPATH_CAPA_MIN_TX         PORT_XPATH "/port-property" PORT_CAPA_MIN_TX
#define PORT_UP_THR                    "/port-up-los-threshold"
#define PORT_XPATH_OPS_UP_THR          PORT_XPATH PORT_UP_THR
#define PORT_LOW_THR                   "/port-low-los-threshold"
#define PORT_XPATH_OPS_LOW_THR         PORT_XPATH PORT_LOW_THR
#define PORT_THR_HYS                   "/port-los-threshold-hys"
#define PORT_XPATH_OPS_THR_HYS         PORT_XPATH PORT_THR_HYS
#define PORT_XPATH_EDFA_ID_I           COD_XPATH_PREFIX "/ports[port-id='%d']/port/edfa-id"
#define PORT_XPATH_EDFA_ID_ALL_I       COD_XPATH_PREFIX "/ports/port/edfa-id"
#define PORT_XPATH_PORT_TYPE_I         COD_XPATH_PREFIX "/ports[port-id='%d']/port/port-type"
#define PORT_XPATH_PORT_QUAL_TYPE_I    COD_XPATH_PREFIX "/ports[port-id='%d']/port/port-qual-type"
#define PORT_XPATH_DIRECTION_ALL_I     COD_XPATH_PREFIX "/ports/port/port-direction"
#define PORT_XPATH_DIRECTION_I         COD_XPATH_PREFIX "/ports[port-id='%d']/port/port-direction"
#define PORT_XPATH_CAPA_MIN_RX_I       COD_XPATH_PREFIX "/ports[port-id='%d']/port/port-property/port-power-capability-min-rx"
#define PORT_XPATH_CAPA_MAX_RX_I       COD_XPATH_PREFIX "/ports[port-id='%d']/port/port-property/port-power-capability-max-rx"
#define PORT_XPATH_CAPA_MIN_TX_I       COD_XPATH_PREFIX "/ports[port-id='%d']/port/port-property/port-power-capability-min-tx"
#define PORT_XPATH_CAPA_MAX_TX_I       COD_XPATH_PREFIX "/ports[port-id='%d']/port/port-property/port-power-capability-max-tx"

/***************************************
* @define used wavelength info structure
***************************************/
#define CHANNEL_XPATH                  PORT_XPATH "/used-wavelengths[wavelength-number='%d']"
#define CHANNEL_XPATH_ALL              PORT_XPATH "/used-wavelengths//*"
#define CHANNEL_WL_NUMBER              "/wavelength-number"
#define CHANNEL_XPATH_KEY              "/used-wavelengths"
#define CHANNEL_WL_CURRENT_POWER       "/wavelength-current-power"
#define CHANNEL_XPATH_CURRENT_POWER    PORT_XPATH "/used-wavelengths[wavelength-number='%d']" CHANNEL_WL_CURRENT_POWER
#define TARGET_POWER_CHANNEL_XPATH     "/wavelength-target-power"
#define CHANNEL_XPATH_TARGET_POWER     PORT_XPATH "/used-wavelengths[wavelength-number='%d']" TARGET_POWER_CHANNEL_XPATH

extern DIRECTION_EN DSC_PortDirectionToEnum(char *pcStr);
extern char *DSC_DirectionToString(DIRECTION_EN enDirection, char *pcBuf);
extern char *DSC_PortQualTypeToString(PORT_QUAL_TYPE_EN enQualType, char *pcBuf);
extern char *DSC_PortWavelengthTypeToString(PORT_WAVELENGTH_TYPE_EN enQualType, char *pcBuf);
extern PORT_WAVELENGTH_TYPE_EN DSC_PortWavelengthTypeToEnum(char *pcStr);
extern PORT_QUAL_TYPE_EN DSC_PortQualTypeToEnum(char *pcStr);
extern int DSC_GetPortNumber(char *xpath);
extern int DSC_GetChannelNumber(char *xpath);
extern bool DSC_PortExist(uint32_t u32PortId);
extern int DSC_PortDirection(uint32_t u32PortId, DIRECTION_EN *penDirection);
extern int DSC_PortEdfaDirection(uint32_t u32PortId, DIRECTION_EN *penDirection);
extern int DSC_PortQualType(uint32_t u32PortId, PORT_QUAL_TYPE_EN *penType);
extern int DSC_GetPortsCnt(size_t *count);
extern int DSC_GetPortsInfo_Iter(PORT_INFO_ST **ppstPortInfo, size_t *count);
extern int DSC_GetPortsInfo(PORT_INFO_ST **ppstPortInfo, size_t *count);
extern int DSC_GetPortInfo(uint32_t u32PortId, PORT_INFO_ST *pstPortInfo, size_t size);
extern int DSC_GetPortCurrentPower(uint32_t u32PortId, double *pdCurrentPower);
extern int DSC_SetPortTargetPower(uint32_t u32PortId, double dTargetPortPower);
extern int DSC_GetPortUsedChannelsCnt(uint32_t u32PortId, size_t *count);
extern int DSC_GetPortUsedChannelsInfo(uint32_t u32PortId, USED_WAVELENGTHS_ST **ppstChannelInfo, size_t *count);
extern int DSC_GetPortUsedChannelsInfo_Iter(uint32_t u32PortId, USED_WAVELENGTHS_ST **ppstChannelInfo, size_t *count);
extern int DSC_SetChannelTargetPower(uint32_t u32PortId, uint32_t u32ChannelId, double dTargetPower);
extern int DSC_SetPortThreshold(uint32_t u32PortId, double dHighThr, double dLowThr);
extern int DSC_GetPortIdByName(char *port_name, unsigned int *id);
#ifdef __cplusplus
}
#endif

#endif

