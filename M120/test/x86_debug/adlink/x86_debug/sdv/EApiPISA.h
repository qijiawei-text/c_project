/*
 *<KHeader>
 *+=========================================================================
 * I  Project Name: EApiDK Embedded Application Development Kit
 *+=========================================================================
 * I  $HeadURL: https://eapidk.svn.sourceforge.net/svnroot/eapidk/trunk/include/EApiPISA.h $
 *+=========================================================================
 * I   Copyright: Copyright (c) 2009-2010, PICMG
 * I      Author: John Kearney,                  John.Kearney@kontron.com
 * I
 * I     License: All rights reserved. This program and the accompanying
 * I              materials are licensed and made available under the
 * I              terms and conditions of the BSD License which
 * I              accompanies this distribution. The full text of the
 * I              license may be found at
 * I              http://opensource.org/licenses/bsd-license.php
 * I
 * I              THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "
 * I              AS IS" BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS OF
 * I              ANY KIND, EITHER EXPRESS OR IMPLIED.
 * I
 * I Description: Auto Created for EApiPISA.h
 * I
 *+-------------------------------------------------------------------------
 * I
 * I  File Name            : EApiPISA.h
 * I  File Location        : include
 * I  Last committed       : $Revision: 74 $
 * I  Last changed by      : $Author: dethrophes $
 * I  Last changed date    : $Date: 2010-06-23 21:26:50 +0200 (Mi, 23 Jun 2010) $
 * I  ID                   : $Id: EApiPISA.h 74 2010-06-23 19:26:50Z dethrophes $
 * I
 *+=========================================================================
 *</KHeader>
 */

#ifndef _EAPIPISA_H_
#define _EAPIPISA_H_


/*
 *
 *      B O A R D   I N F O M A T I O N   S T R I N G S
 *
 */
/* IDS */
#define EAPI_PISA_ID_BOARD_MANUFACTURER_STR     EAPI_ID_BOARD_MANUFACTURER_STR
#define EAPI_PISA_ID_BOARD_NAME_STR             EAPI_ID_BOARD_NAME_STR
#define EAPI_PISA_ID_BOARD_SERIAL_STR           EAPI_ID_BOARD_SERIAL_STR
#define EAPI_PISA_ID_BOARD_BIOS_REVISION_STR    EAPI_ID_BOARD_BIOS_REVISION_STR
#define EAPI_PISA_ID_BOARD_PLATFORM_TYPE_STR    EAPI_ID_BOARD_PLATFORM_TYPE_STR

#define EAPI_PISA_PLATFORM_STR     "PISA"

/*
 *
 *      B O A R D   I N F O M A T I O N   V A L U E S
 *
 */
/* IDS */
#define EAPI_PISA_ID_BOARD_BOOT_COUNTER_VAL        EAPI_ID_BOARD_BOOT_COUNTER_VAL
#define EAPI_PISA_ID_BOARD_RUNNING_TIME_METER_VAL  EAPI_ID_BOARD_RUNNING_TIME_METER_VAL
#define EAPI_PISA_ID_BOARD_PNPID_VAL               EAPI_ID_BOARD_PNPID_VAL
#define EAPI_PISA_ID_BOARD_PLATFORM_REV_VAL        EAPI_ID_BOARD_PLATFORM_REV_VAL


#define EAPI_PISA_ID_BOARD_DRIVER_VERSION_VAL    EAPI_ID_BOARD_DRIVER_VERSION_VAL
#define EAPI_PISA_ID_BOARD_LIB_VERSION_VAL       EAPI_ID_BOARD_LIB_VERSION_VAL

#define EAPI_PISA_REV_1_0      EAPI_VER_CREATE(1, 0, 0)


#define EAPI_PISA_ID_HWMON_CPU_TEMP      EAPI_ID_HWMON_CPU_TEMP
#define EAPI_PISA_ID_HWMON_CHIPSET_TEMP  EAPI_ID_HWMON_CHIPSET_TEMP
#define EAPI_PISA_ID_HWMON_SYSTEM_TEMP   EAPI_ID_HWMON_SYSTEM_TEMP

#define EAPI_PISA_ID_HWMON_VOLTAGE_VCORE EAPI_ID_HWMON_VOLTAGE_VCORE
#define EAPI_PISA_ID_HWMON_VOLTAGE_2V5   EAPI_ID_HWMON_VOLTAGE_2V5
#define EAPI_PISA_ID_HWMON_VOLTAGE_3V3   EAPI_ID_HWMON_VOLTAGE_3V3
#define EAPI_PISA_ID_HWMON_VOLTAGE_VBAT  EAPI_ID_HWMON_VOLTAGE_VBAT
#define EAPI_PISA_ID_HWMON_VOLTAGE_5V    EAPI_ID_HWMON_VOLTAGE_5V
#define EAPI_PISA_ID_HWMON_VOLTAGE_5VSB  EAPI_ID_HWMON_VOLTAGE_5VSB
#define EAPI_PISA_ID_HWMON_VOLTAGE_12V   EAPI_ID_HWMON_VOLTAGE_12V

#define EAPI_PISA_ID_HWMON_FAN_CPU       EAPI_ID_HWMON_FAN_CPU
#define EAPI_PISA_ID_HWMON_FAN_SYSTEM    EAPI_ID_HWMON_FAN_SYSTEM

/*
 *
 *      B A C K L I G H T
 *
 */
/* IDS */
#define EAPI_PISA_ID_BACKLIGHT_1              EAPI_ID_BACKLIGHT_1
#define EAPI_PISA_ID_BACKLIGHT_2              EAPI_ID_BACKLIGHT_2


/*
 *
 *      S T O R A G E
 *
 */
/* IDs */
#define EAPI_PISA_ID_STORAGE_STD              EAPI_ID_STORAGE_STD

/*
 *
 *      I 2 C
 *
 */
/* IDs */
#define EAPI_PISA_ID_I2C_EXTERNAL       EAPI_ID_I2C_EXTERNAL
#define EAPI_PISA_ID_I2C_LVDS_1         EAPI_ID_I2C_LVDS_1
#define EAPI_PISA_ID_I2C_LVDS_2         EAPI_ID_I2C_LVDS_2


/*
 *
 *      G P I O
 *
 */
/* IDs */
/*
 * No GPIOs Supported
 *
 */





#endif /* _EAPIPISA_H_ */
