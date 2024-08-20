#ifndef _DSC_PLATFORM_H
#define _DSC_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

/* com-oplink-platform                                                                            */
#define MODULE_COP                                     "com-oplink-platform"
#define COP_XPATH_COMPONENTS_I                         "/" MODULE_COP ":components"
#define COP_XPATH_COMPONENTLIST_I                      COP_XPATH_COMPONENTS_I "/component//*"
#define COP_XPATH_COMPONENT_ALL_I                      COP_XPATH_COMPONENTS_I "/component"
#define COP_XPATH_COMPONENT_NAME_I                     COP_XPATH_COMPONENTS_I "/component/name"

/* openconfig-platform                                                                            */
#define MODULE_OCP                                     "openconfig-platform"
#define OCP_XPATH_COMPONENTS                           "/" MODULE_OCP ":components"
#define OCP_XPATH_COMPONENTLIST                        OCP_XPATH_COMPONENTS "/component//*"
#define OCP_XPATH_COMPONENTS_ALL                       OCP_XPATH_COMPONENTS "/component"
#define OCP_XPATH_COMPONENTS_NAME                      OCP_XPATH_COMPONENTS "/component/name"

#define LEAF_COMPONENT_NAME                            "]/name"
#define LEAF_SUBCOMPONENT_NAME                         "]/name"

#define OCP_XPATH_COMPONENT "/openconfig-platform:components/component[name='%s']"
#define OCP_XPATH_COMPONENT_FMD "/openconfig-platform:components/component[name='FMD']"
#define OCP_XPATH_COMPONENT_A "/openconfig-platform:components/component"
#define OCP_XPATH_COMPONENT_ALL_CONFIG OCP_XPATH_COMPONENT_A "/config//."
#define OCP_XPATH_COMPONENT_CONFIG                     OCP_XPATH_COMPONENT "/config"
#define OCP_XPATH_COMPONENT_CONFIG_NAME                OCP_XPATH_COMPONENT "/config/name"
#define COMPONENT_CONFIG_NAME                          "/name"

#define OCP_XPATH_COMPONENT_STATE                      OCP_XPATH_COMPONENT "/state"
#define OCP_XPATH_COMPONENT_STATE_ALL                  OCP_XPATH_COMPONENT "/state//*"
#define OCP_XPATH_COMPONENT_STATE_NAME                 OCP_XPATH_COMPONENT "/state/name"
#define OCP_XPATH_COMPONENT_STATE_TYPE                 OCP_XPATH_COMPONENT "/state/type"
#define OCP_XPATH_COMPONENT_STATE_ID                   OCP_XPATH_COMPONENT "/state/id"
#define OCP_XPATH_COMPONENT_STATE_LOCATION             OCP_XPATH_COMPONENT "/state/location"
#define OCP_XPATH_COMPONENT_STATE_DESCRIPTION          OCP_XPATH_COMPONENT "/state/description"
#define OCP_XPATH_COMPONENT_STATE_MFG_NAME             OCP_XPATH_COMPONENT "/state/mfg-name"
#define OCP_XPATH_COMPONENT_STATE_MFG_DATE             OCP_XPATH_COMPONENT "/state/mfg-date"
#define OCP_XPATH_COMPONENT_STATE_HW_VER               OCP_XPATH_COMPONENT "/state/hardware-version"
#define OCP_XPATH_COMPONENT_STATE_FW_VER               OCP_XPATH_COMPONENT "/state/firmware-version"
#define OCP_XPATH_COMPONENT_STATE_SW_VER               OCP_XPATH_COMPONENT "/state/software-version"
#define OCP_XPATH_COMPONENT_STATE_SN                   OCP_XPATH_COMPONENT "/state/serial-no"
#define OCP_XPATH_COMPONENT_STATE_PN                   OCP_XPATH_COMPONENT "/state/part-no"
#define OCP_XPATH_COMPONENT_STATE_REMOVABLE            OCP_XPATH_COMPONENT "/state/removable"
#define OCP_XPATH_COMPONENT_STATE_OPER_STATUS          OCP_XPATH_COMPONENT "/state/oper-status"
#define OCP_XPATH_COMPONENT_STATE_EMPTY                OCP_XPATH_COMPONENT "/state/empty"
#define OCP_XPATH_COMPONENT_STATE_PARENT               OCP_XPATH_COMPONENT "/state/parent"
#define OCP_XPATH_COMPONENT_STATE_TEMP                 OCP_XPATH_COMPONENT "/state/temperature"
#define OCP_XPATH_COMPONENT_STATE_TEMP_ALL             OCP_XPATH_COMPONENT "/state/temperature//*"
#define OCP_XPATH_COMPONENT_STATE_TEMP_INSTANT         OCP_XPATH_COMPONENT "/state/temperature/instant"
#define OCP_XPATH_COMPONENT_STATE_TEMP_AVG             OCP_XPATH_COMPONENT "/state/temperature/avg"
#define OCP_XPATH_COMPONENT_STATE_TEMP_MIN             OCP_XPATH_COMPONENT "/state/temperature/min"
#define OCP_XPATH_COMPONENT_STATE_TEMP_MAX             OCP_XPATH_COMPONENT "/state/temperature/max"
#define OCP_XPATH_COMPONENT_STATE_TEMP_INTERVAL        OCP_XPATH_COMPONENT "/state/temperature/interval"
#define OCP_XPATH_COMPONENT_STATE_TEMP_MIN_TIME        OCP_XPATH_COMPONENT "/state/temperature/min-time"
#define OCP_XPATH_COMPONENT_STATE_TEMP_MAX_TIME        OCP_XPATH_COMPONENT "/state/temperature/max-time"
#define OCP_XPATH_COMPONENT_STATE_TEMP_ALM_STATUS      OCP_XPATH_COMPONENT "/state/temperature/alarm-status"
#define OCP_XPATH_COMPONENT_STATE_TEMP_ALM_THRESHOLD   OCP_XPATH_COMPONENT "/state/temperature/alarm-threshold"
#define OCP_XPATH_COMPONENT_STATE_TEMP_ALM_SEVERITY    OCP_XPATH_COMPONENT "/state/temperature/alarm-severity"
#define OCP_XPATH_COMPONENT_STATE_MEMORY               OCP_XPATH_COMPONENT "/state/memory"
#define OCP_XPATH_COMPONENT_STATE_MEMORY_ALL           OCP_XPATH_COMPONENT "/state/memory//*"
#define OCP_XPATH_COMPONENT_STATE_MEMORY_AVAILABLE     OCP_XPATH_COMPONENT "/state/memory/available"
#define OCP_XPATH_COMPONENT_STATE_MEMORY_UTILIZED      OCP_XPATH_COMPONENT "/state/memory/utilized"
#define OCP_XPATH_COMPONENT_STATE_ALLOCATED_POWER      OCP_XPATH_COMPONENT "/state/allocated-power/"
#define OCP_XPATH_COMPONENT_STATE_USED_POWER           OCP_XPATH_COMPONENT "/state/used-power/"

#define OCP_EXT_LEDS                                   "com-oplink-platform-ext:leds"
#define OCP_XPATH_COMPONENT_STATE_LEDS                 OCP_XPATH_COMPONENT "/state/com-oplink-platform-ext:leds"
#define OCP_XPATH_COMPONENT_STATE_LED_NAME             OCP_XPATH_COMPONENT "/state/com-oplink-platform-ext:leds/led[name=\"%s\"]/name"
#define OCP_XPATH_COMPONENT_STATE_LED_FUNCTION         OCP_XPATH_COMPONENT "/state/com-oplink-platform-ext:leds/led[name=\"%s\"]/led-function"
#define OCP_XPATH_COMPONENT_STATE_LED_STATUS           OCP_XPATH_COMPONENT "/state/com-oplink-platform-ext:leds/led[name=\"%s\"]/led-status"
#define OCP_XPATH_LEDS_ALL                             OCP_XPATH_COMPONENT_A "/state/com-oplink-platform-ext:leds/led//."
#define COMPONENT_STATE_LED_NAME                       "name"
#define COMPONENT_STATE_LED_FUCTION                    "led-function"
#define COMPONENT_STATE_LED_STATUS                     "led-status"

#define OCP_XPATH_COMPONENT_STATE_UNIT_NAME             OCP_XPATH_COMPONENT "/state/com-oplink-platform-ext:unit-name"
#define OCP_XPATH_COMPONENT_STATE_FC_NUMBER             OCP_XPATH_COMPONENT "/state/com-oplink-platform-ext:fc-number"
#define OCP_XPATH_COMPONENT_STATE_PIU_ISSUE_NUMBER      OCP_XPATH_COMPONENT "/state/com-oplink-platform-ext:piu-issue-number"
#define OCP_XPATH_COMPONENT_STATE_FUJITSU_PART_NUMBER   OCP_XPATH_COMPONENT "/state/com-oplink-platform-ext:fujitsu-part-number"
#define DEF_OCP_XPATH_COMPONENT_STATE_FUJITSU_VENDOR    OCP_XPATH_COMPONENT "/state/com-oplink-platform-ext:fujitsu-vendor-name"
#define DEF_OCP_XPATH_COMPONENT_STATE_FUJITSU_SN        OCP_XPATH_COMPONENT "/state/com-oplink-platform-ext:fujitsu-serial-number"
#define DEF_OCP_XPATH_COMPONENT_STATE_CLEI              OCP_XPATH_COMPONENT "/state/com-oplink-platform-ext:clei"
#define DEF_OCP_XPATH_COMPONENT_STATE_MODEL             OCP_XPATH_COMPONENT "/state/com-oplink-platform-ext:model"
#define DEF_OCP_XPATH_COMPONENT_STATE_CPLD_VER          OCP_XPATH_COMPONENT "/state/com-oplink-platform-ext:cpld-version"
#define DEF_OCP_XPATH_COMPONENT_STATE_FPGA_VER          OCP_XPATH_COMPONENT "/state/com-oplink-platform-ext:fpga-version"
#define DEF_OCP_XPATH_COMPONENT_STATE_CONFIG_TYPE       OCP_XPATH_COMPONENT "/state/com-oplink-platform-ext:chassis-type"

#define OCP_XPATH_COMPONENT_PROPERTIES                 OCP_XPATH_COMPONENT "/properties//*"
#define OCP_XPATH_COMPONENT_PROPERTIE                  OCP_XPATH_COMPONENT "/properties/property[name='%s']"
#define OCP_XPATH_COMPONENT_PROPERTIE_ALL_CONFIG       OCP_XPATH_COMPONENT_A "/properties/property/config//*"
#define OCP_XPATH_COMPONENT_PROPERTIE_CONFIG           OCP_XPATH_COMPONENT_PROPERTIE "/config"
#define OCP_XPATH_COMPONENT_PROPERTIE_CONFIG_NAME      OCP_XPATH_COMPONENT_PROPERTIE_CONFIG "/name"
#define COMPONENT_PROPERTIE_CONFIG_NAME                "/name"
#define OCP_XPATH_COMPONENT_PROPERTIE_CONFIG_VALUE     OCP_XPATH_COMPONENT_PROPERTIE_CONFIG "/value"
#define COMPONENT_PROPERTIE_CONFIG_VALUE               "/value"

#define OCP_XPATH_COMPONENT_PROPERTIE_STATE            OCP_XPATH_COMPONENT_PROPERTIE "/state"
#define OCP_XPATH_COMPONENT_PROPERTIE_STATE_NAME       OCP_XPATH_COMPONENT_PROPERTIE_STATE "/name"
#define OCP_XPATH_COMPONENT_PROPERTIE_STATE_VALUE      OCP_XPATH_COMPONENT_PROPERTIE_STATE "/value"
#define OCP_XPATH_COMPONENT_PROPERTIE_STATE_CONFIGURABLE  OCP_XPATH_COMPONENT_PROPERTIE_STATE "/configurable"

#define OCP_XPATH_COMPONENT_SUBCOMPINENTS              OCP_XPATH_COMPONENT "/subcomponents//*"
#define OCP_XPATH_COMPONENT_SUBCOMPINENT_ALL           OCP_XPATH_COMPONENT "/subcomponents/subcomponent"
#define OCP_XPATH_COMPONENT_SUBCOMPINENT_ALL_CONFIG    OCP_XPATH_COMPONENT_A "/subcomponents/subcomponent" "/config//*"
#define OCP_XPATH_COMPONENT_SUBCOMPINENT               OCP_XPATH_COMPONENT "/subcomponents/subcomponent[name='%s']"
#define OCP_XPATH_COMPONENT_SUBCOMPINENT_NAME          OCP_XPATH_COMPONENT "/subcomponents/subcomponent/name" /* OCP_XPATH_COMPONENT_SUBCOMPINENT "/name"        */
#define OCP_XPATH_COMPONENT_SUBCOMPINENT_CONFIG        OCP_XPATH_COMPONENT_SUBCOMPINENT "/config"
#define OCP_XPATH_COMPONENT_SUBCOMPINENT_CONFIG_NAME   OCP_XPATH_COMPONENT_SUBCOMPINENT_CONFIG "/name"
#define COMPONENT_SUBCOMPINENT_CONFIG_NAME             "/name"
#define OCP_XPATH_COMPONENT_SUBCOMPINENT_STATE         OCP_XPATH_COMPONENT_SUBCOMPINENT "/state"

#define OCP_XPATH_COMPONENT_CHASSIS                    OCP_XPATH_COMPONENT "/chassis"
#define OCP_XPATH_COMPONENT_CHASSIS_CONFIG             OCP_XPATH_COMPONENT "/chassis/config"
#define OCP_XPATH_COMPONENT_CHASSIS_ALL_CONFIG         OCP_XPATH_COMPONENT_A "/chassis/config//."
#define OCP_XPATH_COMPONENT_CHASSIS_STATE              OCP_XPATH_COMPONENT "/chassis/state"
#define OCP_XPATH_COMPONENT_PORT                       OCP_XPATH_COMPONENT "/port/"
#define OCP_XPATH_COMPONENT_PORT_CONFIG                OCP_XPATH_COMPONENT "/port/config"
#define OCP_XPATH_COMPONENT_PORT_ALL_CONFIG            OCP_XPATH_COMPONENT_A "/port/config"
#define OCP_XPATH_COMPONENT_PORT_STATE                 OCP_XPATH_COMPONENT "/port/state"
#define OCP_XPATH_COMPONENT_POWER_SUPPLY               OCP_XPATH_COMPONENT "/power-supply"
#define OCP_XPATH_COMPONENT_POWER_SUPPLY_CONFIG        OCP_XPATH_COMPONENT "/power-supply/config"
#define OCP_XPATH_COMPONENT_POWER_SUPPLY_ALL_CONFIG    OCP_XPATH_COMPONENT_A "/power-supply/config//*"
#define OCP_XPATH_COMPONENT_POWER_SUPPLY_CONFIG_ENABLED OCP_XPATH_COMPONENT "/power-supply/config/openconfig-platform-psu:enabled"
#define OCP_XPATH_COMPONENT_POWER_SUPPLY_STATE         OCP_XPATH_COMPONENT "/power-supply/state"
#define OCP_XPATH_COMPONENT_POWER_SUPPLY_ALL_STATE     OCP_XPATH_COMPONENT_A "/power-supply/state"
#define OCP_XPATH_COMPONENT_POWER_SUPPLY_STATE_ALL     OCP_XPATH_COMPONENT "/power-supply/state//*"
#define OCP_XPATH_COMPONENT_POWER_SUPPLY_STATE_CAP     OCP_XPATH_COMPONENT "/power-supply/state/openconfig-platform-psu:capacity"
#define OCP_XPATH_COMPONENT_POWER_SUPPLY_STATE_IN_CUR  OCP_XPATH_COMPONENT "/power-supply/state/openconfig-platform-psu:input-current"
#define OCP_XPATH_COMPONENT_POWER_SUPPLY_STATE_IN_VOL  OCP_XPATH_COMPONENT "/power-supply/state/openconfig-platform-psu:input-voltage"
#define OCP_XPATH_COMPONENT_POWER_SUPPLY_STATE_OUT_CUR OCP_XPATH_COMPONENT "/power-supply/state/openconfig-platform-psu:output-current"
#define OCP_XPATH_COMPONENT_POWER_SUPPLY_STATE_OUT_VOL OCP_XPATH_COMPONENT "/power-supply/state/openconfig-platform-psu:output-voltage"
#define OCP_XPATH_COMPONENT_POWER_SUPPLY_STATE_OUT_PWR OCP_XPATH_COMPONENT "/power-supply/state/openconfig-platform-psu:output-power"
#define OCP_XPATH_COMPONENT_FAN                        OCP_XPATH_COMPONENT "/fan"
#define OCP_XPATH_COMPONENT_FAN_CONFIG                 OCP_XPATH_COMPONENT "/fan/config"
#define OCP_XPATH_COMPONENT_FAN_ALL_CONFIG             OCP_XPATH_COMPONENT_A "/fan/config//."
#define OCP_XPATH_COMPONENT_FAN_STATE                  OCP_XPATH_COMPONENT "/fan/state"
#define OCP_XPATH_COMPONENT_FAN_ALL_STATE              OCP_XPATH_COMPONENT_A "/fan/state"

#define OCP_XPATH_COMPONENT_FMD_STATE               "/openconfig-platform:components/component[name='FMD']" "/state"

#define OCP_XPATH_COMPONENT_FMD_CPU                 "/openconfig-platform:components/component[name='SCC']" "/cpu"

#ifdef _NR1004_
#define DEF_OCP_XPATH_COMPONENT_FMD_CPU             "/openconfig-platform:components/component[name='NR1004']" "/cpu"
#else
#define DEF_OCP_XPATH_COMPONENT_FMD_CPU             "/openconfig-platform:components/component[name='OLSP']" "/cpu"
#endif

#define OCP_XPATH_COMPONENT_FGA1_STATE              "/openconfig-platform:components/component[name='FGA1']" "/state"
#define OCP_XPATH_COMPONENT_FGA2_STATE              "/openconfig-platform:components/component[name='FGA2']" "/state"
#define OCP_XPATH_COMPONENT_FAN0_STATE              "/openconfig-platform:components/component[name='FAN0']" "/fan/state"
#define OCP_XPATH_COMPONENT_FAN1_STATE              "/openconfig-platform:components/component[name='FAN1']" "/fan/state"
#define OCP_XPATH_COMPONENT_FAN2_STATE              "/openconfig-platform:components/component[name='FAN2']" "/fan/state"
#define OCP_XPATH_COMPONENT_FAN3_STATE              "/openconfig-platform:components/component[name='FAN3']" "/fan/state"
/* #define OCP_XPATH_COMPONENT_PSU1_STATE              "/openconfig-platform:components/component[name='PSU1']" "/power-supply/state"*/
/* #define OCP_XPATH_COMPONENT_PSU2_STATE              "/openconfig-platform:components/component[name='PSU2']" "/power-supply/state"*/
#define OCP_XPATH_COMPONENT_PSU1_STATE              "/openconfig-platform:components/component[name='PSU1']"
#define OCP_XPATH_COMPONENT_PSU2_STATE              "/openconfig-platform:components/component[name='PSU2']"
#define OCP_XPATH_COMPONENT_ADD2_COM1_STATE         "/openconfig-platform:components/component[name='Add2_Com1']" "/port/openconfig-transport-line-common:optical-port/state"
#define OCP_XPATH_COMPONENT_DROP2_COM1_STATE        "/openconfig-platform:components/component[name='Drop2_Com1']" "/port/openconfig-transport-line-common:optical-port/state"
#define OCP_XPATH_COMPONENT_ADD2_COM2_STATE         "/openconfig-platform:components/component[name='Add2_Com2']" "/port/openconfig-transport-line-common:optical-port/state"
#define OCP_XPATH_COMPONENT_DROP2_COM2_STATE        "/openconfig-platform:components/component[name='Drop2_Com2']" "/port/openconfig-transport-line-common:optical-port/state"
#define OCP_XPATH_COMPONENT_ADD1_COM1_STATE         "/openconfig-platform:components/component[name='Add1_Com1']" "/port/openconfig-transport-line-common:optical-port/state"
#define OCP_XPATH_COMPONENT_DROP1_COM1_STATE        "/openconfig-platform:components/component[name='Drop1_Com1']" "/port/openconfig-transport-line-common:optical-port/state"
#define OCP_XPATH_COMPONENT_ADD1_COM2_STATE         "/openconfig-platform:components/component[name='Add1_Com2']" "/port/openconfig-transport-line-common:optical-port/state"
#define OCP_XPATH_COMPONENT_DROP1_COM2_STATE        "/openconfig-platform:components/component[name='Drop1_Com2']" "/port/openconfig-transport-line-common:optical-port/state"
#define OCP_XPATH_COMPONENT_SFP_STATE               "/openconfig-platform:components/component[name='SFP']" "/openconfig-platform-transceiver:transceiver/state"

/**************************************************************************************************/
/* definition of component state xpath                                                            */
/**************************************************************************************************/
/* @ {                                                                                            */
#ifdef _NR1004_
#define DEF_OCP_XPATH_COMPONENT_CHASSIS_STATE       "/openconfig-platform:components/component[name='NR1004']" "/state"
#elif defined(_OCS_)
#define DEF_OCP_XPATH_COMPONENT_CHASSIS_STATE       "/openconfig-platform:components/component[name='OCS']" "/state"
#else
#define DEF_OCP_XPATH_COMPONENT_CHASSIS_STATE       "/openconfig-platform:components/component[name='DIABLO']" "/state"
#endif

#define DEF_OCP_XPATH_COMPONENT_SCC_STATE           "/openconfig-platform:components/component[name='SCC']" "/state"
#define DEF_OCP_XPATH_COMPONENT_AMP_STATE           "/openconfig-platform:components/component[name='AMP']" "/state"
#define DEF_OCP_XPATH_COMPONENT_AMP_P_STATE         "/openconfig-platform:components/component[name='AMP-P']" "/state"
#define DEF_OCP_XPATH_COMPONENT_OPS_STATE           "/openconfig-platform:components/component[name='OPS']" "/state"
#define DEF_OCP_XPATH_COMPONENT_RPC_STATE           "/openconfig-platform:components/component[name='RPC']" "/state"
#define DEF_OCP_XPATH_COMPONENT_FAN0_STATE          "/openconfig-platform:components/component[name='FAN0']" "/state"
#define DEF_OCP_XPATH_COMPONENT_FAN1_STATE          "/openconfig-platform:components/component[name='FAN1']" "/state"
#define DEF_OCP_XPATH_COMPONENT_FAN2_STATE          "/openconfig-platform:components/component[name='FAN2']" "/state"
#define DEF_OCP_XPATH_COMPONENT_FAN3_STATE          "/openconfig-platform:components/component[name='FAN3']" "/state"
#define DEF_OCP_XPATH_COMPONENT_PSU1_STATE          "/openconfig-platform:components/component[name='PSU1']" "/state"
#define DEF_OCP_XPATH_COMPONENT_PSU2_STATE          "/openconfig-platform:components/component[name='PSU2']" "/state"

#define DEF_OCP_XPATH_COMPONENT_OPS_4_A_STATE       "/openconfig-platform:components/component[name='OPS-4-A']" "/state"
#define DEF_OCP_XPATH_COMPONENT_OPS_4_B_STATE       "/openconfig-platform:components/component[name='OPS-4-B']" "/state"

#define DEF_OCP_XPATH_COMPONENT_LC_OCS_STATE        "/openconfig-platform:components/component[name='LC-OCS']" "/state"


/* @ }                                                                                            */

/**************************************************************************************************/
/* definition of component key                                                            */
/**************************************************************************************************/
/* @ {
 */
#ifdef _NR1004_
  #define DEF_OCP_COMPONENT_KEY_CHASSIS             "NR1004"
#elif defined(_OCS_)
  #define DEF_OCP_COMPONENT_KEY_CHASSIS             "OCS"
#else
  #define DEF_OCP_COMPONENT_KEY_CHASSIS             "OLSP"
#endif

#define DEF_OCP_COMPONENT_KEY_SCC                   "SCC"
#define DEF_OCP_COMPONENT_KEY_AMP                   COMPONENT_KEY_AMP
#define DEF_OCP_COMPONENT_KEY_AMP_P                 COMPONENT_KEY_AMP_P
#define DEF_OCP_COMPONENT_KEY_OPS                   COMPONENT_KEY_OPS
#define DEF_OCP_COMPONENT_KEY_OCS                   COMPONENT_KEY_OCS
#define DEF_OCP_COMPONENT_KEY_RPC                   "RPC"
#define DEF_OCP_COMPONENT_KEY_FAN0                  "FAN0"
#define DEF_OCP_COMPONENT_KEY_FAN1                  "FAN1"
#define DEF_OCP_COMPONENT_KEY_FAN2                  "FAN2"
#define DEF_OCP_COMPONENT_KEY_FAN3                  "FAN3"
#define DEF_OCP_COMPONENT_KEY_PSU1                  "PSU1"
#define DEF_OCP_COMPONENT_KEY_PSU2                  "PSU2"
#define DEF_OCP_COMPONENT_KEY_OPS_4_A               COMPONENT_KEY_OPS_4_A
#define DEF_OCP_COMPONENT_KEY_OPS_4_B               COMPONENT_KEY_OPS_4_B
#define DEF_OCP_COMPONENT_KEY_OSC                   COMPONENT_KEY_OSC
#define DEF_OCP_COMPONENT_KEY_OSC_P                 COMPONENT_KEY_OSC_P
#define DEF_OCP_COMPONENT_KEY_LC_OCS                "LC-OCS"

/* @ }                                                                                            */

#define OCP_XPATH_COMPONENT_FAN_STATE_ALL              OCP_XPATH_COMPONENT "/fan/state//*"
#define OCP_XPATH_COMPONENT_FAN_STATE_SPEED            OCP_XPATH_COMPONENT "/fan/state/openconfig-platform-fan:speed"
#define OCP_XPATH_COMPONENT_FABRIC                     OCP_XPATH_COMPONENT "/fabric/"
#define OCP_XPATH_COMPONENT_FABRIC_CONFIG              OCP_XPATH_COMPONENT "/fabric/config"
#define OCP_XPATH_COMPONENT_FABRIC_STATE               OCP_XPATH_COMPONENT "/fabric/state"
#define OCP_XPATH_COMPONENT_STORAGE                    OCP_XPATH_COMPONENT "/storage/"
#define OCP_XPATH_COMPONENT_STORAGE_CONFIG             OCP_XPATH_COMPONENT "/storage/config"
#define OCP_XPATH_COMPONENT_STORAGE_ALL_CONFIG         OCP_XPATH_COMPONENT_A "/storage/config//."
#define OCP_XPATH_COMPONENT_STORAGE_STATE              OCP_XPATH_COMPONENT "/storage/state"
#define OCP_XPATH_COMPONENT_CPU                        OCP_XPATH_COMPONENT "/cpu"
#define OCP_XPATH_COMPONENT_CPU_CONFIG                 OCP_XPATH_COMPONENT "/cpu/config"
#define OCP_XPATH_COMPONENT_CPU_ALL_CONFIG             OCP_XPATH_COMPONENT_A "/cpu/config//."
#define OCP_XPATH_COMPONENT_CPU_STATE                  OCP_XPATH_COMPONENT "/cpu/openconfig-platform-cpu:utilization/state"
#define OCP_XPATH_COMPONENT_CPU_STATE_ALL              OCP_XPATH_COMPONENT "/cpu/openconfig-platform-cpu:utilization/state//*"
#define OCP_XPATH_COMPONENT_CPU_STATE_INSTANT          OCP_XPATH_COMPONENT_CPU_STATE "/instant"
#define OCP_XPATH_COMPONENT_CPU_STATE_AVG              OCP_XPATH_COMPONENT_CPU_STATE "/avg"
#define OCP_XPATH_COMPONENT_CPU_STATE_MAX              OCP_XPATH_COMPONENT_CPU_STATE "/max"
#define OCP_XPATH_COMPONENT_CPU_STATE_MIN              OCP_XPATH_COMPONENT_CPU_STATE "/min"
#define OCP_XPATH_COMPONENT_CPU_STATE_INTERVAL         OCP_XPATH_COMPONENT_CPU_STATE "/interval"
#define OCP_XPATH_COMPONENT_INTEGTATED_CIRCUIT         OCP_XPATH_COMPONENT "/integrated-circuit/"
#define OCP_XPATH_COMPONENT_INTEGTATED_CIRCUIT_CONFIG  OCP_XPATH_COMPONENT "/integrated-circuit/config"
#define OCP_XPATH_COMPONENT_INTEGTATED_CIRCUIT_STATE   OCP_XPATH_COMPONENT "/integrated-circuit/state"
#define OCP_XPATH_COMPONENT_BACKPLANE                  OCP_XPATH_COMPONENT "/backplane/"
#define OCP_XPATH_COMPONENT_BACKPLANE_CONFIG           OCP_XPATH_COMPONENT "/backplane/config"
#define OCP_XPATH_COMPONENT_BACKPLANE_ALL_CONFIG       OCP_XPATH_COMPONENT_A "/backplane/config//."
#define OCP_XPATH_COMPONENT_BACKPLANE_STATE            OCP_XPATH_COMPONENT "/backplane/state"

#define OCP_XPATH_COMPONENT_TRANSCEIVER                OCP_XPATH_COMPONENT "/openconfig-platform-transceiver:transceiver"
#define OCP_XPATH_COMPONENT_TRANSCEIVER_CONFIG           OCP_XPATH_COMPONENT_TRANSCEIVER "/config"
#define OCP_XPATH_COMPONENT_TRANSCEIVER_CONFIG_ENABLED   OCP_XPATH_COMPONENT_TRANSCEIVER "/config/enabled"
#define OCP_XPATH_COMPONENT_TRANSCEIVER_STATE            OCP_XPATH_COMPONENT_TRANSCEIVER "/openconfig-platform-transceiver:state"
#define OCP_XPATH_COMPONENT_TRANSCEIVER_STATE_ALL        OCP_XPATH_COMPONENT_TRANSCEIVER "/openconfig-platform-transceiver:state//*"
#define OCP_XPATH_COMPONENT_TRANSCEIVER_STATE_ENABLED    OCP_XPATH_COMPONENT_TRANSCEIVER "/state/enabled"
#define OCP_XPATH_COMPONENT_TRANSCEIVER_STATE_INPUT_POWER    OCP_XPATH_COMPONENT_TRANSCEIVER "/state/input-power"
#define OCP_XPATH_COMPONENT_TRANSCEIVER_STATE_INPUT_POWER_INSTANT    OCP_XPATH_COMPONENT_TRANSCEIVER "/state/input-power/instant"
#define OCP_XPATH_COMPONENT_TRANSCEIVER_STATE_OUTPUT_POWER    OCP_XPATH_COMPONENT_TRANSCEIVER "/state/output-power"
#define OCP_XPATH_COMPONENT_TRANSCEIVER_STATE_OUTPUT_POWER_INSTANT    OCP_XPATH_COMPONENT_TRANSCEIVER "/state/output-power/instant"


#define OCP_PLATFORM_LEAF_NAME                         "name"
#define OCP_PLATFORM_LEAF_CONFIG                       "config"
#define OCP_PLATFORM_LEAF_STATE                        "state"
#define OCP_PLATFORM_LEAF_PROPERTIES                   "properties"
#define OCP_PLATFORM_LEAF_SUBCOMPONENTS                "subcomponents"
#define OCP_PLATFORM_LEAF_CHASSIS                      "chassis"
#define OCP_PLATFORM_LEAF_POWERSUPPLY                  "power-supply"
#define OCP_PLATFORM_LEAF_PORT                         "port"
#define OCP_PLATFORM_LEAF_FAN                          "fan"
#define OCP_PLATFORM_LEAF_FABRIC                       "fabric"
#define OCP_PLATFORM_LEAF_STORAGE                      "storage"
#define OCP_PLATFORM_LEAF_CPU                          "cpu"
#define OCP_PLATFORM_LEAF_IC                           "integrated-circuit"
#define OCP_PLATFORM_LEAF_BACKPLANE                    "backplane"
#define OCP_PLATFORM_LEAF_TEMPERATURE                  "temperature"
#define OCP_PLATFORM_LEAF_MEMORY                       "memory"
#define OCP_PLATFORM_LEAF_TRANSCEIVER                  "openconfig-platform-transceiver:transceiver"

#define LEAF_COMPONENT_ID                              "]/state/id"
#define LEAF_COMPONENT_TYPE                            "]/state/type"
#define LEAF_COMPONENT_LOCATION                        "]/state/location"
#define LEAF_COMPONENT_DESC                            "]/state/description"
#define LEAF_COMPONENT_MFG_NAME                        "]/state/mfg-name"
#define LEAF_COMPONENT_MFG_DATA                        "]/state/mfg-date"
#define LEAF_COMPONENT_HW_VER                          "]/state/hardware-version"
#define LEAF_COMPONENT_FW_VER                          "]/state/firmware-version"
#define LEAF_COMPONENT_SW_VER                          "]/state/software-version"
#define LEAF_COMPONENT_SN                              "]/state/serial-no"
#define LEAF_COMPONENT_PN                              "]/state/part-no"

#define LEAF_COMPONENT_UNIT_NAME                       "]/state/com-oplink-platform-ext:unit-name"
#define LEAF_COMPONENT_FC_NUMBER                       "]/state/com-oplink-platform-ext:fc-number"
#define LEAF_COMPONENT_PIU_ISSUE_NUMBER                "]/state/com-oplink-platform-ext:piu-issue-number"
#define LEAF_COMPONENT_FUJITSU_PART_NUMBER             "]/state/com-oplink-platform-ext:fujitsu-part-number"
#define DEF_LEAF_COMPONENT_FUJITSU_VENDOR              "]/state/com-oplink-platform-ext:fujitsu-vendor-name"
#define DEF_LEAF_COMPONENT_FUJITSU_SN                  "]/state/com-oplink-platform-ext:fujitsu-serial-number"
#define DEF_LEAF_COMPONENT_CLEI                        "]/state/com-oplink-platform-ext:clei"
#define DEF_LEAF_COMPONENT_MODEL                       "]/state/com-oplink-platform-ext:model"
#define DEF_LEAF_COMPONENT_CPLD_VER                    "]/state/com-oplink-platform-ext:cpld-version"
#define DEF_LEAF_COMPONENT_FPGA_VER                    "]/state/com-oplink-platform-ext:fpga-version"


#define LEAF_COMPONENT_REMOVE                          "]/state/removable"
#define LEAF_COMPONENT_OPER_STATUS                     "]/state/oper-status"
#define LEAF_COMPONENT_EMPTY                           "]/state/empty"
#define LEAF_COMPONENT_PARENT                          "]/state/parent"
#define LEAF_COMPONENT_TEMP_INSTANT                    "]/state/temperature/instant"
#define LEAF_COMPONENT_TEMP_AVG                        "]/state/temperature/avg"
#define LEAF_COMPONENT_TEMP_MIN                        "]/state/temperature/min"
#define LEAF_COMPONENT_TEMP_MAX                        "]/state/temperature/max"
#define LEAF_COMPONENT_TEMP_INTERVAL                   "]/state/temperature/interval"
#define LEAF_COMPONENT_MEM_AVAIL                       "]/state/memory/available"
#define LEAF_COMPONENT_MEM_UTIL                        "]/state/memory/utilized"
#define LEAF_COMPONENT_CPU_INSTANT                     "]/cpu/openconfig-platform-cpu:utilization/state/instant"
#define LEAF_COMPONENT_CPU_INTERVAL                    "]/cpu/openconfig-platform-cpu:utilization/state/interval"
#define LEAF_COMPONENT_FAN_SPEED                       "]/fan/state/openconfig-platform-fan:speed"
#define LEAF_COMPONENT_PSU_CAP                         "]/power-supply/state/openconfig-platform-psu:capacity"
#define LEAF_COMPONENT_PSU_IN_CUR                      "]/power-supply/state/openconfig-platform-psu:input-current"
#define LEAF_COMPONENT_PSU_IN_VOL                      "]/power-supply/state/openconfig-platform-psu:input-voltage"
#define LEAF_COMPONENT_PSU_OUT_CUR                     "]/power-supply/state/openconfig-platform-psu:output-current"
#define LEAF_COMPONENT_PSU_OUT_VOL                     "]/power-supply/state/openconfig-platform-psu:output-voltage"
#define LEAF_COMPONENT_PSU_OUT_POWER                   "]/power-supply/state/openconfig-platform-psu:output-power"
#define LEAF_COMPONENT_TRANSCEIVER_ENABLED             "]/openconfig-platform-transceiver:transceiver/state/enabled"
#define LEAF_COMPONENT_TRANSCEIVER_IN_POWER_INST       "]/openconfig-platform-transceiver:transceiver/state/input-power/instant"
#define LEAF_COMPONENT_TRANSCEIVER_OUT_POWER_INST      "]/openconfig-platform-transceiver:transceiver/state/output-power/instant"


#define OCP_XPATH_COMPONENT_OPTICAL_PORT               OCP_XPATH_COMPONENT "/port/openconfig-transport-line-common:optical-port"
#define OCP_XPATH_COMPONENT_OPTICAL_PORT_STATE         OCP_XPATH_COMPONENT "/port/openconfig-transport-line-common:optical-port/state"
#define OCP_XPATH_COMPONENT_OPTICAL_PORT_ALL_STATE     OCP_XPATH_COMPONENT_A "/port/openconfig-transport-line-common:optical-port/state"
#define OCP_XPATH_COMPONENT_OPTICAL_PORT_INPUT         OCP_XPATH_COMPONENT "/port/openconfig-transport-line-common:optical-port/state/input-power"
#define OCP_XPATH_COMPONENT_OPTICAL_PORT_OUTPUT        OCP_XPATH_COMPONENT "/port/openconfig-transport-line-common:optical-port/state/output-power"
#define OCP_XPATH_COMPONENT_OPTICAL_PORT_INPUT_INST    OCP_XPATH_COMPONENT "/port/openconfig-transport-line-common:optical-port/state/input-power/instant"
#define OCP_XPATH_COMPONENT_OPTICAL_PORT_INPUT_AVG     OCP_XPATH_COMPONENT "/port/openconfig-transport-line-common:optical-port/state/input-power/avg"
#define OCP_XPATH_COMPONENT_OPTICAL_PORT_INPUT_MIN     OCP_XPATH_COMPONENT "/port/openconfig-transport-line-common:optical-port/state/input-power/min"
#define OCP_XPATH_COMPONENT_OPTICAL_PORT_INPUT_MAX     OCP_XPATH_COMPONENT "/port/openconfig-transport-line-common:optical-port/state/input-power/max"
#define OCP_XPATH_COMPONENT_OPTICAL_PORT_INPUT_INTERVAL     OCP_XPATH_COMPONENT "/port/openconfig-transport-line-common:optical-port/state/input-power/interval"
#define OCP_XPATH_COMPONENT_OPTICAL_PORT_INPUT_MIN_TIME     OCP_XPATH_COMPONENT "/port/openconfig-transport-line-common:optical-port/state/input-power/min-time"
#define OCP_XPATH_COMPONENT_OPTICAL_PORT_INPUT_MAX_TIME     OCP_XPATH_COMPONENT "/port/openconfig-transport-line-common:optical-port/state/input-power/max-time"

#define OCP_XPATH_COMPONENT_OPTICAL_PORT_OUTPUT_INST   OCP_XPATH_COMPONENT "/port/openconfig-transport-line-common:optical-port/state/output-power/instant"
#define OCP_XPATH_COMPONENT_OPTICAL_PORT_OUTPUT_AVG    OCP_XPATH_COMPONENT "/port/openconfig-transport-line-common:optical-port/state/output-power/avg"
#define OCP_XPATH_COMPONENT_OPTICAL_PORT_OUTPUT_MIN    OCP_XPATH_COMPONENT "/port/openconfig-transport-line-common:optical-port/state/output-power/min"
#define OCP_XPATH_COMPONENT_OPTICAL_PORT_OUTPUT_MAX    OCP_XPATH_COMPONENT "/port/openconfig-transport-line-common:optical-port/state/output-power/max"
#define OCP_XPATH_COMPONENT_OPTICAL_PORT_OUTPUT_INTERVAL     OCP_XPATH_COMPONENT "/port/openconfig-transport-line-common:optical-port/state/output-power/interval"
#define OCP_XPATH_COMPONENT_OPTICAL_PORT_OUTPUT_MIN_TIME     OCP_XPATH_COMPONENT "/port/openconfig-transport-line-common:optical-port/state/output-power/min-time"
#define OCP_XPATH_COMPONENT_OPTICAL_PORT_OUTPUT_MAX_TIME     OCP_XPATH_COMPONENT "/port/openconfig-transport-line-common:optical-port/state/output-power/max-time"
#define OCP_XPATH_COMPONENT_OPTICAL_PORT_INPUT_LOW_SEC       OCP_XPATH_COMPONENT "/port/openconfig-transport-line-common:optical-port/state/com-alibaba-platform-ext:input-power-low-second"
#define OCP_XPATH_COMPONENT_OPTICAL_PORT_INPUT_LOS_SEC       OCP_XPATH_COMPONENT "/port/openconfig-transport-line-common:optical-port/state/com-alibaba-platform-ext:input-power-los-second"


#define OCP_XPATH_COMPONENT_OPTICAL_PORT_LIST          OCP_XPATH_COMPONENTS "/component/port/openconfig-transport-line-common:optical-port/openconfig-transport-line-common:state//*"
#define LEAF_OPTICAL_PORT_INPUT_POWER                      "input-power"
#define LEAF_OPTICAL_PORT_OUTPUT_POWER                     "output-power"
#define LEAF_OPTICAL_PORT_INSTANT                      "/instant"
#define LEAF_OPTICAL_PORT_POWER_INSTANT                "power/instant"
#define LEAF_OPTICAL_PORT_INPUT_POWER_INSTANT          "input-power/instant"
#define LEAF_OPTICAL_PORT_OUTPUT_POWER_INSTANT         "output-power/instant"
#define LEAF_OPTICAL_PORT_INPUT_POWER_LOW_SEC          "input-power-low-second"
#define LEAF_OPTICAL_PORT_INPUT_POWER_LOS_SEC          "input-power-los-second"

#define COMPONENT_KEY_HCF                              "HCF"
#define COMPONENT_KEY_NR1004_CHAS_01                   "NR1004-CHAS-01"
#define COMPONENT_KEY_NR1004                           "NR1004"
#define COMPONENT_KEY_DIABLO                           "DIABLO"
#define COMPONENT_KEY_OLSP                             "OLSP"
#define COMPONENT_KEY_OPS                              "OPS"
#define COMPONENT_KEY_AMP                              "AMP"
#define COMPONENT_KEY_AMP_P                            "AMP-P"
#define COMPONENT_KEY_OPS_4_A                          "OPS-4-A"
#define COMPONENT_KEY_OPS_4_B                          "OPS-4-B"
#define COMPONENT_KEY_OLS_HP                           "OLS-HP"

#define COMPONENT_KEY_PA                               "PA"
#define COMPONENT_KEY_BA                               "BA"
#define COMPONENT_KEY_OCM                              "OCM"
#define COMPONENT_KEY_OTDR                             "OTDR"
#define COMPONENT_KEY_OSC                              "OSC"
#define COMPONENT_KEY_OCS                              "OCS"
#define COMPONENT_KEY_OCS_32X32                        "OCS-32X32"

#define COMPONENT_KEY_PA_P                             "PA-P"
#define COMPONENT_KEY_BA_P                             "BA-P"
#define COMPONENT_KEY_OCM_P                            "OCM-P"
#define COMPONENT_KEY_OTDR_P                           "OTDR-P"
#define COMPONENT_KEY_OSC_P                            "OSC-P"

#define COMPONENT_KEY_FMD                              "FMD"
#define COMPONENT_KEY_LINECARD                         "LINECARD-1-1"
#define COMPONENT_KEY_VOA_BA                           "VOA-BA"
#define COMPONENT_KEY_VOA_PA                           "VOA-PA"
#define COMPONENT_KEY_VOA_BA_P                         "VOA-BA-P"
#define COMPONENT_KEY_VOA_PA_P                         "VOA-PA-P"
#define COMPONENT_KEY_VOA_1                            "VOA1"
#define COMPONENT_KEY_VOA_2                            "VOA2"
#define COMPONENT_KEY_WSS_1                            "WSS1"
#define COMPONENT_KEY_WSS_2                            "WSS2"
#define COMPONENT_KEY_AMP_1                            "FGA1"               /* "edfa-1"                                                */
#define COMPONENT_KEY_AMP_2                            "FGA2"               /* "edfa-2"                                                */

/* Lband ComIn and ComOut                                                                         */
#define COMPONENT_KET_COM_IN                            "ComIn"
#define COMPONENT_KET_COM_OUT                           "ComOut"

#define COMPONENT_KEY_SFP                               "SFP"

/* openconfig-platform                                                                            */
#define OPENCONFIG_HARDWARE_COMPONENT_CHASSIS                    "CHASSIS"
#define OPENCONFIG_HARDWARE_COMPONENT_BACKPLANE                  "BACKPLANE"
#define OPENCONFIG_HARDWARE_COMPONENT_FABRIC                     "FABRIC"
#define OPENCONFIG_HARDWARE_COMPONENT_POWER_SUPPLY               "POWER_SUPPLY"
#define OPENCONFIG_HARDWARE_COMPONENT_FAN                        "FAN"
#define OPENCONFIG_HARDWARE_COMPONENT_SENSOR                     "SENSOR"
#define OPENCONFIG_HARDWARE_COMPONENT_FRU                        "FRU"
#define OPENCONFIG_HARDWARE_COMPONENT_LINECARD                   "LINECARD"
#define OPENCONFIG_HARDWARE_COMPONENT_CONTROLLER_CARD            "CONTROLLER_CARD"
#define OPENCONFIG_HARDWARE_COMPONENT_PORT                       "PORT"
#define OPENCONFIG_HARDWARE_COMPONENT_TRANSCEIVER                "TRANSCEIVER"
#define OPENCONFIG_HARDWARE_COMPONENT_CPU                        "CPU"
#define OPENCONFIG_HARDWARE_COMPONENT_STORAGE                    "STORAGE"
#define OPENCONFIG_HARDWARE_COMPONENT_CHASSINTEGRATED_CIRCUITIS  "INTEGRATED_CIRCUIT"
#define OPENCONFIG_SOFTWARE_COMPONENT_OPERATING_SYSTEM           "OPERATING_SYSTEM"
#define COMPONENT_OPER_STATUS_ACTIVE                             "openconfig-platform-types:ACTIVE"
#define COMPONENT_OPER_STATUS_INACTIVE                           "openconfig-platform-types:INACTIVE"
#define COMPONENT_OPER_STATUS_DISABLED                           "openconfig-platform-types:DISABLED"

#define DEF_OCP_MFG_DATE                  "2000-01-01"


/*port*/
#define OCP_XPATH_COMPONENT_CLIENT_IN_STATE          "/openconfig-platform:components/component[name='client-in']/port/openconfig-transport-line-common:optical-port/state/output-power/instant"
#define OCP_XPATH_COMPONENT_CLIENT_OUT_STATE         "/openconfig-platform:components/component[name='client-out']/port/openconfig-transport-line-common:optical-port/state/output-power/instant"
#define OCP_XPATH_COMPONENT_LINE_IN_STATE            "/openconfig-platform:components/component[name='line-in']/port/openconfig-transport-line-common:optical-port/state/input-power/instant"
#define OCP_XPATH_COMPONENT_LINE_OUT_STATE           "/openconfig-platform:components/component[name='line-out']/port/openconfig-transport-line-common:optical-port/state/output-power/instant"

#define OCP_XPATH_COMPONENT_CLIENT_IN_P_STATE        "/openconfig-platform:components/component[name='client-in-P']/port/openconfig-transport-line-common:optical-port/state/output-power/instant"
#define OCP_XPATH_COMPONENT_CLIENT_OUT_P_STATE       "/openconfig-platform:components/component[name='client-out-P']/port/openconfig-transport-line-common:optical-port/state/output-power/instant"
#define OCP_XPATH_COMPONENT_LINE_IN_P_STATE          "/openconfig-platform:components/component[name='line-in-P']/port/openconfig-transport-line-common:optical-port/state/input-power/instant"
#define OCP_XPATH_COMPONENT_LINE_OUT_P_STATE         "/openconfig-platform:components/component[name='line-out-P']/port/openconfig-transport-line-common:optical-port/state/output-power/instant"

#define OCP_XPATH_COMPONENT_OPS_W_IN_STATE           "/openconfig-platform:components/component[name='ops-w-in']/port/openconfig-transport-line-common:optical-port/state/input-power/instant"
#define OCP_XPATH_COMPONENT_OPS_P_IN_STATE           "/openconfig-platform:components/component[name='ops-p-in']/port/openconfig-transport-line-common:optical-port/state/input-power/instant"
#define OCP_XPATH_COMPONENT_OPS_W_OUT_STATE          "/openconfig-platform:components/component[name='ops-w-out']/port/openconfig-transport-line-common:optical-port/state/output-power/instant"
#define OCP_XPATH_COMPONENT_OPS_P_OUT_STATE          "/openconfig-platform:components/component[name='ops-p-out']/port/openconfig-transport-line-common:optical-port/state/output-power/instant"
#define OCP_XPATH_COMPONENT_MUX_IN_STATE             "/openconfig-platform:components/component[name='mux-in']/port/openconfig-transport-line-common:optical-port/state/input-power/instant"
#define OCP_XPATH_COMPONENT_MUX_OUT_STATE            "/openconfig-platform:components/component[name='mux-out']/port/openconfig-transport-line-common:optical-port/state/output-power/instant"

#define OCP_XPATH_COMPONENT_PRI_IN_STATE             "/openconfig-platform:components/component[name='PriIn']/port/openconfig-transport-line-common:optical-port/state/input-power/instant"
#define OCP_XPATH_COMPONENT_SEC_IN_STATE             "/openconfig-platform:components/component[name='SecIn']/port/openconfig-transport-line-common:optical-port/state/input-power/instant"
#define OCP_XPATH_COMPONENT_PRI_OUT_STATE            "/openconfig-platform:components/component[name='PriOut']/port/openconfig-transport-line-common:optical-port/state/output-power/instant"
#define OCP_XPATH_COMPONENT_SEC_OUT_STATE            "/openconfig-platform:components/component[name='SecOut']/port/openconfig-transport-line-common:optical-port/state/output-power/instant"
#define OCP_XPATH_COMPONENT_COM_IN_STATE             "/openconfig-platform:components/component[name='ComIn']/port/openconfig-transport-line-common:optical-port/state/input-power/instant"
#define OCP_XPATH_COMPONENT_COM_OUT_STATE            "/openconfig-platform:components/component[name='ComOut']/port/openconfig-transport-line-common:optical-port/state/output-power/instant"

#define OCP_XPATH_COMPONENT_PORT_X_PRI_IN_STATE             "/openconfig-platform:components/component[name='PriIn-%d']/port/openconfig-transport-line-common:optical-port/state/input-power/instant"
#define OCP_XPATH_COMPONENT_PORT_X_SEC_IN_STATE             "/openconfig-platform:components/component[name='SecIn-%d']/port/openconfig-transport-line-common:optical-port/state/input-power/instant"
#define OCP_XPATH_COMPONENT_PORT_X_PRI_OUT_STATE            "/openconfig-platform:components/component[name='PriOut-%d']/port/openconfig-transport-line-common:optical-port/state/output-power/instant"
#define OCP_XPATH_COMPONENT_PORT_X_SEC_OUT_STATE            "/openconfig-platform:components/component[name='SecOut-%d']/port/openconfig-transport-line-common:optical-port/state/output-power/instant"
#define OCP_XPATH_COMPONENT_PORT_X_COM_IN_STATE             "/openconfig-platform:components/component[name='ComIn-%d']/port/openconfig-transport-line-common:optical-port/state/input-power/instant"
#define OCP_XPATH_COMPONENT_PORT_X_COM_OUT_STATE            "/openconfig-platform:components/component[name='ComOut-%d']/port/openconfig-transport-line-common:optical-port/state/output-power/instant"

typedef struct st_OC_COMPONENT_CONFIG
{
    char acName[32];
} ST_OC_COMPONENT_CONFIG;

typedef struct st_OC_COMPONENT_STATE_TEMPERATURE
{
    double dInstant;
    double dAvg;
    double dMin;
    double dMax;
    uint64_t ullInterval;                                                   /* units nanoseconds;                                      */
    bool bAlarmStatus;
    uint32_t ulAlarmThreshold;
    char acAlarmSeverity[32];
} ST_OC_COMPONENT_STATE_TEMPERATURE;

typedef struct st_OC_COMPONENT_STATE_MEMORY
{
    uint64_t ullAvailable;
    uint64_t ullUtilized;
} ST_OC_COMPONENT_STATE_MEMORY;

#define CP_PORT_NUM_MAX     (32)
#define DEF_FJT_VENDOR_NAME "Fujitsu" /* PSU need this information */
typedef struct st_OC_COMPONENT_STATE
{
    char acName[64];
    char acType[64];
    char acId[64];
    char acLocation[64];
    char acDescription[64];
    char acMfgName[128];
    char acMfgDate[128];
    char acHardwareVersion[128];
    char acFirmwareVersion[128];
    char acSoftwareVersion[128];
    char acSerialNo[128];
    char acPartNo[128];
    bool bRemovable;
    char acOperStatus[64];
    bool bEmpty;
    char acParent[64];
    ST_OC_COMPONENT_STATE_TEMPERATURE stTemper;
    ST_OC_COMPONENT_STATE_MEMORY stMem;
    uint32_t ulAllocatedPower;
    uint32_t ulUsedPower;
    char acUnitName[128];
    char acFcNumber[128];
    char acPiuIssueNumber[128];
    char acFujitsuPartNumber[128];
    char acFpgaVer[128];
    char acCpldVer[128];
    char acModel[128];          /* This is for FJT request, just from openROADM yang */
    char acClei[128];          /* This is for FJT request, just from openROADM yang */
    char acFjtVendorName[128]; /* This is for FJT request,PSU do NOT need this information */
    char acFjtSn[128]; /* This is for FJT request,PSU do NOT need this information */
    char acConfigType[128];
} ST_OC_COMPONENT_STATE;

typedef struct st_OC_COMPONENT_PROPERTIES
{
    char acName[32];                                                        /* key                                                     */
} ST_OC_COMPONENT_PROPERTIES;


#define OCP_SUBCOMPONENT_NUM_MAX     (256)
typedef struct st_OC_COMPONENT_SUBCOMPONENT
{
    char acName[32];                                                        /* key                                                     */
} ST_OC_COMPONENT_SUBCOMPONENT;

typedef struct st_OC_SUBCOMOPNENT_LIST {
    ST_OC_COMPONENT_SUBCOMPONENT astSubcomponent[OCP_SUBCOMPONENT_NUM_MAX];
    size_t nTotal;
} ST_OC_SUBCOMPONENT_LIST;

typedef struct st_OC_COMPONENT_CHASSIS
{
    char acName[32];                                                        /* key                                                     */
} ST_OC_COMPONENT_CHASSIS;

typedef struct  st_OPTICAL_POWER
{
    double dInstant;
    double dAvg;
    double dMin;
    double dMax;
    uint64_t ulInterval;
    uint64_t ulMinTime;
    uint64_t ulMaxTime;
    uint64_t ulLosSecond;
    uint64_t ulLowSecond;
} ST_OPTICAL_POWER;

typedef struct st_OC_COMPONENT_PORT
{
    char acName[32];                                                        /* key                                                     */
    ST_OPTICAL_POWER stOptiaclPower;
} ST_OC_COMPONENT_PORT;

typedef struct st_OC_COMPONENT_POWER_SUPPLY
{
    char acName[32];                                                        /* key                                                     */
    char acCapacity[64];
    char acInputCurrent[64];
    char acInputVoltage[64];
    char acOutputCurrent[64];
    char acOutputVoltage[64];
    char acOutputPower[64];
} ST_OC_COMPONENT_POWER_SUPPLY;

typedef struct st_OC_COMPONENT_FAN
{
    char acName[32];                                                        /* key                                                     */
    uint32_t ulSpeed;
} ST_OC_COMPONENT_FAN;

typedef struct st_OC_COMPONENT_FABRIC
{
    char acName[32];                                                        /* key                                                     */
} ST_OC_COMPONENT_FABRIC;

typedef struct st_OC_COMPONENT_STORAGE
{
    char acName[32];                                                        /* key                                                     */
} ST_OC_COMPONENT_STORAGE;

typedef struct st_OC_COMPONENT_CPU
{
    char acName[32];                                                        /* key                                                     */
    uint8_t ucInstant;
    uint8_t ucAvg;
    uint8_t ucMin;
    uint8_t ucMax;
    uint64_t ullInterval;                                                   /* nanoseconds                                             */
} ST_OC_COMPONENT_CPU;

typedef struct st_OC_COMPONENT_IC
{
    char acName[32];                                                        /* key                                                     */
} ST_OC_COMPONENT_IC;

typedef struct st_OC_COMPONENT_BACKPLANE
{
    char acName[32];                                                        /* key                                                     */
} ST_OC_COMPONENT_BACKPLANE;

typedef struct st_OC_COMPONENT_TRANSCEIVER
{
    char acName[32];                                                        /* key                                                     */
    bool bEnabled;
    double dInputPowerInstant;
    double dOutputPowerInstant;
} ST_OC_COMPONENT_TRANSCEIVER;

#define OCP_COMPONENT_NAME_XPATH_LEN_MAX       (128)                        /* change from 64 to 128 for component LINECARD-1-1        */
#define OCP_COMPONENT_NUM_MAX       (516)

typedef struct st_OC_COMPONENT
{
    char acName[32];                                                        /* key                                                     */
    ST_OC_COMPONENT_CONFIG stConfig;
    ST_OC_COMPONENT_STATE stState;
    ST_OC_COMPONENT_SUBCOMPONENT stSubcomponent[OCP_SUBCOMPONENT_NUM_MAX];
    ST_OC_COMPONENT_PROPERTIES stProperties[OCP_SUBCOMPONENT_NUM_MAX];
    ST_OC_COMPONENT_CHASSIS stChassis;
    ST_OC_COMPONENT_PORT stPort;
    ST_OC_COMPONENT_POWER_SUPPLY stPowerSupply;
    ST_OC_COMPONENT_FAN stFan;
    ST_OC_COMPONENT_FABRIC stFabric;
    ST_OC_COMPONENT_STORAGE stStorage;
    ST_OC_COMPONENT_CPU stCpu;
    ST_OC_COMPONENT_IC stIc;
    ST_OC_COMPONENT_BACKPLANE stBackplane;
    ST_OC_COMPONENT_TRANSCEIVER stTransceiver;
    int iSubCount;
} ST_OC_COMPONENT;

/* openconfig-platform                                                                            */
int DSC_OCP_SetComponentSlotStatus(char *pcCpName, bool bSlotOnline);
int DSC_OCP_SetComponentMfgInfo(char *pcCpName, ST_OC_COMPONENT_STATE *pstComponentState);
int DSC_OCP_SetComponentState(char *pcCpName, ST_OC_COMPONENT_STATE *pstComponentState);
int DSC_OCP_GetComponentState(char *pcComponentName, ST_OC_COMPONENT_STATE *pstComponentInfo);

int DSC_OCP_GetCpuByName(char *pcComponentName, ST_OC_COMPONENT_CPU *pstComponentInfo);
int DSC_OCP_GetFanByName(char *pcComponentName, ST_OC_COMPONENT_FAN *pstComponentInfo);
int DSC_OCP_GetPsuByName(char *pcComponentName, ST_OC_COMPONENT_POWER_SUPPLY *pstComponentInfo);
int DSC_OCP_GetMemByName(char *pcComponentName, ST_OC_COMPONENT_STATE_MEMORY *pstComponentInfo);
int DSC_OCP_GetTempByName(char *pcComponentName, ST_OC_COMPONENT_STATE_TEMPERATURE *pstComponentInfo);

int DSC_OCP_GetSubComponentsCountAndName(char *pcComponentName, ST_OC_COMPONENT_SUBCOMPONENT *pstSubComponent, size_t *ptSubCompntCount);
int DSC_OCP_GetComponents(ST_OC_COMPONENT **ppstCpComponent, size_t *ptCompntCount);
int DSC_OCP_GetSubComponents(char *pcComponentName, ST_OC_COMPONENT_SUBCOMPONENT **ppstSubComponent, size_t *ptSubCompntCount);
int DSC_OCP_GetComponentsCountAndName(ST_OC_COMPONENT *pstComponent, size_t *ptCount);
int DSC_OCP_GetAll(ST_OC_COMPONENT **ppCompnent, size_t *ptCount);

int DSC_OCP_SetPsuEnable(char *pcComponentName, bool enabled);
int DSC_OCP_GetSfpConfig(char *pcComponentName, bool *pEnabled);
int DSC_OCP_SetSfpConfig(char *pcComponentName, bool bEnabled);
int DSC_OCP_GetSfpAll(char *pcComponentName, ST_OC_COMPONENT_TRANSCEIVER *pstTransceiver);
int DSC_OCP_GetOpticalPortPowerInfo(ST_OC_COMPONENT_PORT *pstPorts, size_t *ptCount);
int DSC_OCP_GetOpticalPortPowerOutInfo(ST_OC_COMPONENT_PORT *pstPorts, size_t *ptCount);
int DSC_OCP_SetChassisType(char *pcConfigType);

bool DSC_OCP_IsComponentExist(const char *pcComponentName);

/* com-oplink-platform-ext.yang                                                                   */
#define DSC_EXT_LED_STATUS_RED_STR               "RED"
#define DSC_EXT_LED_STATUS_GREEN_STR             "GREEN"
#define DSC_EXT_LED_STATUS_RED_FLASH_STR         "RED_FLASH"
#define DSC_EXT_LED_STATUS_GREEN_FLASH_STR       "GREEN_FLASH"
#define DSC_EXT_LED_STATUS_RED_GREEN_FLASH_STR   "RED_GREEN_FLASH"
#define DSC_EXT_LED_STATUS_OFF_STR               "OFF"
#define DSC_EXT_LED_STATUS_ABNORMAL_STR          "ABNORMAL"
#define DSC_EXT_LED_STATUS_AMBER_STR             "AMBER"
#define DSC_EXT_LED_STATUS_AMBER_FLASH_STR       "AMBER_FLASH"
#define DSC_EXT_LED_STATUS_LAMPTEST_STR          "LAMPTEST"

#define DSC_EXT_LED_OPERATION_ENABLED_STR        "ENABLED"
#define DSC_EXT_LED_OPERATION_DISABLED_STR       "DISABLED"

#define MODULE_NS_OCP_EXT                        "com-oplink-platform-ext"
#define RPC_XPATH_LED_OPERTION                   "/" MODULE_NS_OCP_EXT ":led-operation"
#define RPC_XPATH_LED_OPER_OPERTION              RPC_XPATH_LED_OPERTION "/operation"
#define RPC_XPATH_LED_OPER_INTERVAL              RPC_XPATH_LED_OPERTION "/interval"
#define RPC_XPATH_LED_OPER_SRC_COMPONENT         RPC_XPATH_LED_OPERTION "/source-component"
#define RPC_XPATH_LED_OPER_SRC_LED               RPC_XPATH_LED_OPERTION "/source-led"
#define RPC_XPATH_LED_OPER_MESSAGE               RPC_XPATH_LED_OPERTION "/message"

#define FILE_LAMPTEST_RUNNING_FLAG               "/tmp/lamptest_running_flag"

typedef struct OCP_LED_STATE_st {
    char acName[16];
    char acFuction[64];
    char acStatus[32];
} OCP_LED_STATE_ST;

typedef struct OCP_LED_LIST_st {
    OCP_LED_STATE_ST *pstLedState;
    size_t nTotal;
} OCP_LED_LIST_ST;

typedef struct OCP_LED_OPERATION_st {
    bool bEnabled;
    uint16_t usInterval;
    char acSrcComponent[16];
    char acSrcLed[16];
} OCP_LED_OPERATION_ST;

typedef enum DSC_LED_STATE_en
{
    DSC_LED_STATE_OFF = 0,
    DSC_LED_STATE_RED,
    DSC_LED_STATE_GREEN,
    DSC_LED_STATE_AMBER,
    DSC_LED_STATE_INVALID,                                                  /* not use the led                                         */
    DSC_LED_STATE_RED_BLINK = 5,
    DSC_LED_STATE_GREEN_BLINK,
    DSC_LED_STATE_AMBER_BLINK,
    DSC_LED_STATE_RED_GREEN_BLINK,                                          /* lamptest state (no use in oc2a)                         */
    DSC_LED_STATE_LAMPTEST,                                                 /* lamptest state                                          */
    DSC_LED_STATE_MAX
} DSC_LED_STATE_EN;

#define CP_LED_NUM_MAX 6
typedef struct st_LED_INFO
{
    char acLedName[32];
    DSC_LED_STATE_EN enStatus;
} LED_INFO_ST;

typedef struct st_CP_LED_INFO
{
    char acCpName[32];
    LED_INFO_ST astLedInfo[CP_LED_NUM_MAX];
    uint32_t uiLedNum;
    uint32_t uiSlotId;
} CP_LED_INFO_ST;


/*
   Struction for component name, device type and slot index.
   Members description:
   pcComponentName: Component name.
   ulDevType: Component device type.
   ulSlotIndex: Slot index referenced with HAL layer.
   ulSubIndex: The index for the same type device, eg, FAN/PSU, the sub index of FAN1 is 0, FAN2 is 1.
   ulLedCount: LED count of this component.
 */
typedef struct st_DSC_COMPONENT_INFO
{
    const char *pcComponentName;
    uint32_t ulDevType;
    uint32_t ulSlotIndex;
    uint32_t ulSubIndex;
    uint32_t ulLedCount;
} ST_DSC_COMPONENT_INFO;

char *DSC_OCP_LedStatusToString(uint32_t uiLedStatus, char *pcLedStatus, size_t nLen);
int DSC_RpcLedOperation(OCP_LED_OPERATION_ST *pstLedOper);
int DSC_OCP_GetAllLedState(OCP_LED_LIST_ST *pstLedList);

bool DSC_OCP_IsLamptestRunning();
int DSC_OCP_SetLamptestFlag(bool bEnable);


/* LEDs name define*/
#define LED_NAME_ACT        "ACT"
#define LED_NAME_ALM        "ALM"
#define LED_NAME_CLIENT     "CLIENT"

#define LED_NAME_FAN        "FAN"
#define LED_NAME_FAN0       "FAN0"
#define LED_NAME_FAN1       "FAN1"
#define LED_NAME_FAN2       "FAN2"
#define LED_NAME_FAN3       "FAN3"

#define LED_NAME_LINE       "LINE"
#define LED_NAME_OCM        "OCM"
#define LED_NAME_OTDR       "OTDR"
#define LED_NAME_SYS        "SYS"

#define LED_NAME_COM        "COM"
#define LED_NAME_PRI        "PRI"
#define LED_NAME_SEC        "SEC"
#define LED_NAME_PS         "P/S"

#define LED_NAME_PS_1       "P/S-1"
#define LED_NAME_COM_1      "COM-1"
#define LED_NAME_PRI_1      "PRI-1"
#define LED_NAME_SEC_1      "SEC-1"

#define LED_NAME_PS_2       "P/S-2"
#define LED_NAME_COM_2      "COM-2"
#define LED_NAME_PRI_2      "PRI-2"
#define LED_NAME_SEC_2      "SEC-2"

#define LED_NAME_PS_3       "P/S-3"
#define LED_NAME_COM_3      "COM-3"
#define LED_NAME_PRI_3      "PRI-3"
#define LED_NAME_SEC_3      "SEC-3"

#define LED_NAME_PS_4       "P/S-4"
#define LED_NAME_COM_4      "COM-4"
#define LED_NAME_PRI_4      "PRI-4"
#define LED_NAME_SEC_4      "SEC-4"

#ifdef _NR1004_
#define LED_NAME_CLIENTW    LED_NAME_PRI
#define LED_NAME_CLIENTP    LED_NAME_SEC
#define LED_NAME_MUX        LED_NAME_COM
#define LED_NAME_WP         LED_NAME_PS
#else
#define LED_NAME_CLIENTW    "CLIENT-W"
#define LED_NAME_CLIENTP    "CLIENT-P"
#define LED_NAME_MUX        "MUX"
#define LED_NAME_WP         "W/P"
#endif

#define LED_NAME_OTDR_SAFETY     "OTDR_SAFETY"
#define LED_NAME_LINE_SAFETY     "LINE_SAFETY"

/* Query LED index by component name and led name. */
int DSC_LedNameToIndex(const char *pcComponentName, const char *pcLedStr, uint32_t *pulLedIndex);
/* Query LED name by component name and led index. */
char *DSC_LedIndexToName(const char *pcComponentName, uint32_t ulLedIndex, char *pcLedName, size_t nLen);

/* Query the total number of component information sequence. */
uint32_t DSC_GetComponentInfoCount(void);
/* Query the specified component information by component name. */
const ST_DSC_COMPONENT_INFO *DSC_GetComponentInfoByName(const char *ComponentName);
/* Query the specified component information by sequence index. */
const ST_DSC_COMPONENT_INFO *DSC_GetComponentInfoByIndex(uint32_t ulIndex);
/* Query the specified component information by slot index. */
const ST_DSC_COMPONENT_INFO *DSC_GetComponentInfoBySlotIndex(uint32_t ulSlotIndex);


#define RPC_XPATH_SET_CHASSIS_TYPE_OPERTION   "/" MODULE_NS_OCP_EXT ":set-chassis-type"
#define RPC_XPATH_LEAF_CHASSIS_TYPE           RPC_XPATH_SET_CHASSIS_TYPE_OPERTION "/chassis-type"


#ifdef __cplusplus
}
#endif

#endif

