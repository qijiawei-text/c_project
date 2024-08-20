#ifndef __REST_COMMON_H__
#define __REST_COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>


/* #include "oplk_datatype.h" */
#include "cJSON.h"
#include "DSC_inc.h"


#ifdef __cplusplus
extern "C" {
#endif

#define  DEBUG_REST_PRINT           (0)

#define HTTP_BODY_MAX_LEN           (32 * 1024)   /*POST/PUT: max size of body data length*/
#define URL_MAX_LEN                 (128)

/* HTTP response status code */
typedef enum HTTP_STATUS_CODE_en {
    HTTP_STATUS_CODE_200 = 200,            /* OK */
    HTTP_STATUS_CODE_201 = 201,            /* progress */

    HTTP_STATUS_CODE_299 = 299,            /* OK */

    HTTP_STATUS_CODE_400 = 400,            /* User login failed */
    HTTP_STATUS_CODE_401 = 401,            /* Unauthorized */
    HTTP_STATUS_CODE_403 = 403,            /* User privilege mismatch */
    HTTP_STATUS_CODE_404 = 404,            /* Not Found */
    HTTP_STATUS_CODE_405 = 405,            /* invalid input */
    HTTP_STATUS_CODE_406 = 406,            /* failed operation */
    HTTP_STATUS_CODE_408 = 408,            /* session timeout */
    HTTP_STATUS_CODE_421 = 421,            /* Too Many Connections */
    HTTP_STATUS_CODE_500 = 500,            /* Internal Server Error */

    HTTP_STATUS_CODE_MAX
} HTTP_STATUS_CODE_EN;

/* HTTP request method */
typedef enum REQ_METHOD_en {
    METHOD_GET = 1,
    METHOD_HEAD = 2,
    METHOD_POST = 3,
    METHOD_PUT = 4,
    METHOD_PATCH = 5,
    METHOD_DELETE = 6,
    METHOD_CONNECT = 7,
    METHOD_OPTIONS = 8,
    METHOD_TRACE = 9,
    METHOD_INVALID
} REQ_METHOD_EN;

/* REST request data structure */
typedef struct REST_REQ_st {
    REQ_METHOD_EN enMethod;
    char acUrl[URL_MAX_LEN];
    char acToken[TOKEN_MAX_LEN];
    char *pcBody;
    size_t nBodyLen;
    cJSON *pstQueryParas;
    char acRemoteAddr[40];      /* Host address of HTTP request header */
    char acResv[216];
} REST_REQ_ST;

/* REST response data structure */
typedef struct REST_RESP_st {
    int iStatus;
    char acMessage[256];
    cJSON *pstResponse;
} REST_RESP_ST;

typedef struct REQUEST_METHOD_KV_st
{
    const char *pcMethod;
    REQ_METHOD_EN enMethode;
} REQUEST_METHOD_KV_ST;

typedef void (*PF_REST_HANDLE_FUNC)(REST_REQ_ST *, REST_RESP_ST *);

typedef struct REST_HANDLE_st
{
    REQ_METHOD_EN enMethod;
    char acUrlPath[URL_MAX_LEN];
    bool bUrlMatchExactly;
    unsigned int uiMinAccess;    /* min access level */
    PF_REST_HANDLE_FUNC pfHandlerFunc;
} REST_HANDLE_ST;

#define STR_BASIC_AUTHORIZATION                       "Basic "
#define HTTP_HEADER_AUTHORIZATION                     "Authorization"
#define HTTP_HEADER_DEFAULT_BASIC_AUTHORIZATION       "WWW-Authenticate: Basic realm=Authorization Required"

#define CLEAR_NO_USED_WARNING(x) ((x) = (x))

#define WEIGHT_TEN                (10)
#define WEIGHT_HUNDRED            (100)
#define WEIGHT_THOUSAND           (1000)
#define WEIGHT_TEN_THOUSAND       (10000)

#define GET_2_DECIMAL_DIGITS_OF_DOUBLE(num)  (((double)((long)(((num) + ((num) > 0 ? 0.005 : -0.005)) * WEIGHT_HUNDRED))) / WEIGHT_HUNDRED)
#define GET_3_DECIMAL_DIGITS_OF_DOUBLE(num)  (((double)((long long)(((num) + ((num) > 0 ? 0.0005 : -0.0005)) * WEIGHT_THOUSAND))) / WEIGHT_THOUSAND)
#define GET_4_DECIMAL_DIGITS_OF_DOUBLE(num)  (((double)((long long)(((num) + ((num) > 0 ? 0.00005 : -0.00005)) * WEIGHT_TEN_THOUSAND))) / WEIGHT_TEN_THOUSAND)

#define TAKE_ADDRESS(type, var)  (((type) *)(&(var)))
#define TAKE_VOID_ADDRESS(var)  ((void *)(&(var)))
#define TAKE_VALUE_BY_TYPE(type, left, right)  (*(type *)(left) = (type)(right))

/*only used for comment*/
#define REGION_DES(x)         (1)

#define KEY_PAGE_NUM                              "page"
#define KEY_PAGE_SIZE                             "page_size"

#define  MAX_ATTRIBUTE_PAIR_NUM    (5)

#define CONTENT_TYPE_NONE  0
#define CONTENT_TYPE_JSON  1
#define CONTENT_TYPE_XML   2

#define REST_PRODUCT_NAME_OCS                     "OCS"
#define REST_PRODUCT_NAME_HCF                     "HCF"
#define REST_PRODUCT_NAME_OLSP                    "OLSP"

/* the same with CMD_CTRL_PD_ID_EN in cmd_control.c */
typedef enum REST_PD_ID_en
{
    REST_PD_LINE_IN = 1,
    REST_PD_LINE_OUT = 2,
    REST_PD_BA_IN = 3,
    REST_PD_BA_OUT = 4,
    REST_PD_PA_IN = 5,
    REST_PD_PA_OUT = 6,
    REST_PD_PAM4_ADD = 7,
    REST_PD_PAM4_DROP = 8,
    REST_PD_OSC_ADD = 9,
    REST_PD_OSC_DROP = 10,
    REST_PD_OSC_RX = 11
} REST_PD_ID_EN;

typedef struct PAGE_MGNT_st
{
    int iPage;
    int iPageSize;
    int iLength;
    int iTotal;
}PAGE_MGNT_ST;

typedef enum query_flag_e
{
    P_KEY_START,
    P_VALUE_START
}QUERY_FLAG_E;

typedef struct KEY_VALUE_AV_st
{
    char acKey[32];
    int iType;
    unsigned int uiLen;
}KEY_VALUE_AV_ST;

typedef enum VALUE_TYPE_e
{
    INT_VALUE_TYPE = 1,
    STRING_VALUE_TYPE
}VALUE_TYPE_E;

typedef struct ATTR_PAIR_st
{
    char acKey[32];
    char acValue[32];
}ATTR_PAIR_ST;

typedef struct URL_QUERY_AP_st
{
    unsigned int uiNum;
    ATTR_PAIR_ST stAttrPair[MAX_ATTRIBUTE_PAIR_NUM];
} URL_QUERY_AP_ST;

typedef enum JSON_DATA_TYPE_en
{
    EN_DATA_TYPE_CHAR = 0,
    EN_DATA_TYPE_UCHAR,
    EN_DATA_TYPE_SHORT,
    EN_DATA_TYPE_USHORT,
    EN_DATA_TYPE_INT,
    EN_DATA_TYPE_UINT,
    EN_DATA_TYPE_FLOAT,
    EN_DATA_TYPE_DOUBLE,
    EN_DATA_TYPE_STRING,
    EN_DATA_TYPE_BOOL,
    EN_DATA_TYPE_MAX
} JSON_DATA_TYPE_EN;

typedef struct JSON_OBJ_st
{
    char *pcNodeName;
    int iDataType;           /*refer to JSON_DATA_TYPE_EN*/
    void *pvValue;
    unsigned int uiLen;      /**/
} JSON_OBJ_ST;

typedef struct JSON_OBJ_NEW_st
{
    char *pcNodeName;
    int iDataType;           /*refer to JSON_DATA_TYPE_EN*/
    void *pvValue;
    unsigned int uiLen;      /**/
    bool *pbExist;               /* added on 2020/04/02, true meams node exists, false means not */
} JSON_OBJ_NEW_ST;

typedef struct OPLK_QUEUE_st {
    void *pvData;                /**< Data of the queue. */
    unsigned int uiCapacity;   /**< queue capacity in number of elements. */
    unsigned int uiElemSize;   /**< Size of one element in the queue */
    unsigned int uiHead;       /**< Index of the first element in the queue. */
    unsigned int uiCount;      /**< Number of elements stored in the queue. */
} OPLK_QUEUE_ST;

int GetPageParas(URL_QUERY_AP_ST *pstQueryAP, int *piPage, int *piPageSize);

int GetSinglePara(URL_QUERY_AP_ST *pstQueryAP, const char *pcName, void *pcValue, unsigned int uiLen, VALUE_TYPE_E eType);

int JSON_GetObjItems(const cJSON *pstObjJson, JSON_OBJ_ST *pstJsonObj);
int JSON_GetObjItemsOptional(const cJSON *pstObjJson, JSON_OBJ_NEW_ST *pstJsonObj);

cJSON *JSON_ParseObject(char *pcValue, size_t tValLen);

REQ_METHOD_EN REST_MethodToEnum(char *pcMethod);
const char *REST_MethodToString(REQ_METHOD_EN enMethod);

char *REST_GetFileContentWithEndChar(char *pcFileName, uint32_t *puiFileLen);
int REST_SetEachFlag(void *pstFlag, uint32_t uiLen, bool bFlag);

void REST_SetProductName(const char *pcName);
bool REST_IsProductName(const char *pcName);

#ifdef __cplusplus
}
#endif

#endif

