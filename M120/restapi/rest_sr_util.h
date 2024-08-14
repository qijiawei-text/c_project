#ifndef __REST_SR_UTIL_H__
#define __REST_SR_UTIL_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define JSON_FILE_MODULE_PATH         "/etc/rest/module_path.json"
#define JSON_FILE_NUMBER_TYPE         "/etc/rest/number_type.json"
#define JSON_FILE_STRING_TYPE         "/etc/rest/string_type.json"
#define JSON_FILE_LIST_KEY            "/etc/rest/list_key.json"

typedef struct REST_JSON_CONFIG_st {
    cJSON *pstObjModulePath;
    cJSON *pstObjNumberType;
    cJSON *pstObjStringType;
    cJSON *pstObjListKey;
} REST_JSON_CONFIG_ST;

typedef enum JSON_CONFIG_OBJECT_TYPE_en {
    EN_MODULE_PATH_OBJECT,
    EN_NUMBER_TYPE_OBJECT,
    EN_STRING_TYPE_OBJECT,
    EN_LIST_KEY_OBJECT,
    EN_CONFIG_OBJECT_TYPE_MAX
} JSON_CONFIG_OBJECT_TYPE_EN;

typedef struct LIST_KEY_MAPPING_TABLE_st {
    const char *pcListName;
    const char *pcKeyName;
    const char *pcUniqueKeywords;
} LIST_KEY_MAPPING_TABLE_ST;

int REST_LoadJsonConfig();
void REST_FreeJsonConfig();
cJSON *REST_GetConfigObj(JSON_CONFIG_OBJECT_TYPE_EN enObjType);

int REST_UrlToYangXpath(const char *pcUrl, char *pcYangXpath, uint32_t uiLen);

#ifdef __cplusplus
}
#endif
#endif

