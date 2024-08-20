#ifndef __REST_HANDLE_H__
#define __REST_HANDLE_H__


#ifdef __cplusplus
extern "C"
{
#endif

/******* define url for msft *******/
#define URL_OMT_INVENTORY                   "/omt/inventory"
#define URL_OMT_CONFIG                      "/omt/config"
#define URL_OMT_REMOTE_NODE_INFO            "/omt/remote-node-info"
#define URL_OMT_REMOTE_NODE_INFO_P          "/omt/remote-node-info-p"
#define URL_OMT_CONFIG_PREAMP               URL_OMT_CONFIG "/pre-amp"
#define URL_OMT_CONFIG_BOOSTERAMP           URL_OMT_CONFIG "/booster-amp"
#define URL_OMT_CONFIG_PREAMP_P             URL_OMT_CONFIG "/pre-amp-p"
#define URL_OMT_CONFIG_BOOSTERAMP_P         URL_OMT_CONFIG "/booster-amp-p"
#define URL_OMT_CONFIG_SYSTEM               URL_OMT_CONFIG "/system"
#define URL_OMT_CURRENT_PM_VALUES           "/omt/current-pm-values"
#define URL_OMT_PAST_PM_VALUES              "/omt/past-pm-values"
#define URL_OMT_CURRENT_ALARMS              "/omt/current-alarms"
#define URL_OMT_OPERATIONS_COLDRESTART      "/omt/operations/coldrestart"
#define URL_OMT_OPERATIONS_WARMRESTART      "/omt/operations/warmrestart"
#define URL_OMT_USER                        "/omt/user"
#define URL_OMT_USER_LOGIN                  URL_OMT_USER "/login"
#define URL_OMT_USER_LOGOUT                 URL_OMT_USER "/logout"
#define URL_OMT_USER_ROOT                   URL_OMT_USER "/"
#define URL_OMT_SOFTWARE_UPGRADE            "/omt/software/upgrade"
#define URL_OMT_SOFTWARE_DOWNLOAD           "/omt/software/download"
#define URL_OMT_SOFTWARE_ACTIVATE           "/omt/software/activate"
#define URL_OMT_SLOTS                       "/omt/slots"
#define URL_OMT_OPERATIONS_TRANSFER_FILE    "/omt/operations/transfer-file"
#define URL_OMT_OPERATIONS_TRANSFER_LOG     "/omt/operations/transfer-log"
#define URL_OMT_OPERATIONS_AUTO_GAIN_W      "/omt/operations/auto-gain"
#define URL_OMT_OPERATIONS_AUTO_GAIN_P      "/omt/operations/auto-gain-p"
#define URL_OMT_CONFIG_THRESHOLD            "/omt/config/threshold"
#define URL_OMT_CONFIG_THRESHOLD_P          "/omt/config/threshold-p"
#define URL_OMT_OPERATIONS_LAMP_TEST        "/omt/system/lamptest"
#define URL_OMT_OPERATIONS_APSD             "/omt/system/apsd"
#define URL_OMT_OPERATIONS_APSD_P           "/omt/system/apsd-p"
#define URL_OMT_IPACL                       "/omt/system/ipacl"
#define URL_OMT_MUX_CONFIG                  "/data/optical-mux/config"
#define URL_OMT_MUX_STATE                   "/data/optical-mux/state"
#define URL_OMT_CLEAR_AUTO_ALARMS           "/omt/system/clear-auto-alarms"

#define URL_OMT_CURRENT_PM_15MIN            "/omt/current-pm-15minute"
#define URL_OMT_CURRENT_PM_24HOUR           "/omt/current-pm-24hour"


/******* define molex url from yaml converted by yang  *******/
/*ocm url*/
#define URL_DATA_CHANNEL_MONITORS_ALL       "/data/channel-monitors"
#define URL_DATA_CHANNEL_MONITOR_ALL        "/data/channel-monitors/channel-monitor"  /* not used */
#define URL_DATA_CHANNEL_MONITOR_ONE        "/data/channel-monitors/channel-monitor="  /* not used */

#define URL_DATA_OCM_CONFIG                 "/data/channel-monitors/config"
#define URL_DATA_OCM_STATE                  "/data/channel-monitors/state"

#define URL_OMT_GET_FIBER_CONFIG_BY_TYPE    "/omt/auto-control/fiber-config/type="
#define URL_OMT_GET_FIBER_CONFIG_BY_TYPE_P  "/omt/auto-control/fiber-config/type-p="

#define URL_DATA_OCM_OSA_ONE                "/data/ocm/osa="

/*common prefix url*/
#define URL_DATA_PREFIX                     "/data/"
#define URL_OPT_PREFIX                      "/operations/"

/*ops url*/
#define URL_DATA_OPS_ALL                    "/data/aps/aps-modules"
#define URL_DATA_OPS_CONFIG                 "/data/aps/aps-modules/aps-module=OLP/config"
#define URL_DATA_OPS_STATE                  "/data/aps/aps-modules/aps-module=OLP/state"
#define URL_DATA_OPS_PORTS                  "/data/aps/aps-modules/aps-module=OLP/ports"

/*otdr url*/
#define URL_DATA_OTDR                       URL_DATA_PREFIX "optical-otdr"
/* #define URL_DATA_OTDR_CONFIG                URL_DATA_OTDR "config" */
#define URL_OPT_GET_TRACE_DATA              URL_OPT_PREFIX "get-trace-data"
#define URL_OPT_OTDR_SCAN                   URL_OPT_PREFIX "otdr-scan"
#define URL_OPT_TRACE_BASELINE              URL_OPT_PREFIX "trace-baseline"

#define URL_OTDR_SOR_FILES                  URL_DATA_OTDR "/otdr/sor-files"
#define URL_OTDR_P_SOR_FILES                URL_DATA_OTDR "/otdr-p/sor-files"

/* openconfig-gnoi-otdr */
#define URL_DATA_OPENCONFIG_OTDR            URL_DATA_PREFIX "optical-time-domain-reflectometer"
#define URL_OPT_SET_TRACE_BASELINE          URL_OPT_PREFIX "set-baseline"
#define URL_DATA_OPENCONFIG_OTDR_SOR_FILES  URL_DATA_OPENCONFIG_OTDR "/sor-files"

/* openconfig-transport-line-connectivity */
#define URL_DATA_OPENCONFIG_OCS             URL_DATA_PREFIX "openconfig-transport-line-connectivity"
#define URL_DATA_OCS_CONNECTIONS            URL_DATA_OPENCONFIG_OCS "/connections"

#define URL_DATA_OCS_CONNECTIONS_ALL        "/data/connections"
#define URL_DATA_OCS_CONNECTION             "/data/connections/connection"
#define URL_DATA_OCS_CONNECTION_ONE         "/data/connections/connection="
#define URL_DATA_OCS_CONNECTION_ONE_CONFIG         "/data/connections/connection=%d/config"


/* com-oplink-dfb.yaml */
#define URL_DATA_DFB                        "/data/optical-dfb"

/* openconfig-optical-amplifier.yaml */
#define URL_DATA_OC_AMPLIFIER_ROOT          "/data/optical-amplifier"

/* openconfig-optical-attenuator.yaml */
#define URL_DATA_OC_ATTENUATOR_ROOT         "/data/optical-attenuator"

/* com-oplink-laser-safty.yaml */
#define URL_DATA_LASER_SAFTY_ROOT           "/data/laser-safty"

REST_HANDLE_ST *REST_GetHandle(REQ_METHOD_EN enMethod, const char *pcUrl);
int REST_RegisterApi();
int REST_UnRegisterApi();

#ifdef __cplusplus
}
#endif

#endif
