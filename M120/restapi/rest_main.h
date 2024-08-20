/******************************************************************************
*
* Copyright(C), 2016, OPLINK Tech.Co., Ltd
*
* Filename:      rest_main.h
* Description:   REST APIs processing for Oplink Devices
* Author:        Jimmy and Guangjun Guo
* History:
*     [Author]             [Date]           [Version]    [Description]
* [1] Jimmy and Guangjun Guo   2016/12/07  Ver 1.0.0    Initial file.
*
******************************************************************************/
#ifndef __REST_MAIN_API_H__
#define __REST_MAIN_API_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "mongoose.h"


#define REST_SESSION_WAIT_TIME    (30)

#define HTTP_PORT_DEFAULT                "80"
#define HTTPS_PORT_DEFAULT               "443"
#define HTTP_PORT_8080                   "127.0.0.1:8080"
#define HTTP_PORT_8081                   "127.0.0.1:8081"
#define HTTP_PORT_8082                   "127.0.0.1:8082"
#define HTTP_PORT_8083                   "127.0.0.1:8083"
#define APP_NAME_RESTAPI                 "restapi"

void mg_cp_src_to_dest(const struct mg_str *src, char *pcdest, unsigned int uiLen);

/* void handle_rest_api(struct mg_connection *nc, struct http_message *hm); */

void REST_MainEntry(struct mg_connection *pstMgConn, struct mg_http_message *pstHttpMsg);

int REST_Init(char *pcAppName);
int REST_Stop(char *pcAppName);

#ifdef __cplusplus
}
#endif
#endif

