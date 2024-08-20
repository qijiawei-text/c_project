/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      RegisterFile.h
   Author:        Zhu Weian
   Date:          2020-09-28
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2020-09-28] [1.0] [Creator]


*****************************************************************************************************/

#include "cJSON.h"
#include "SlotManager.h"


class CRegisterFile
{
public:

    CRegisterFile()
    {
    }

    ~CRegisterFile()
    {
    }

    static bool Load();
    static uint32_t ConvertDevType(const std::string &strTypeName);
    static CBoardRegisterInfo ms_stBoardRegInfo;
    static std::vector<CSlotRegInfo> ms_staSlotRegInfo;
    static std::map<uint32_t, CCardInfo> ms_staCardRegInfo;

private:
    static std::map<std::string, uint32_t> ms_staHwTypeMap;
    static std::map<std::string, uint32_t> ms_staCardTypeMap;
    static std::map<std::string, uint32_t> ms_staModuleTypeMap;
    static std::map<std::string, uint32_t> ms_staModuleFuncMap;
    static std::map<std::string, uint32_t> ms_staDevTypeMap;
    static std::map<std::string, uint32_t> ms_staDevFuncMap;
    static std::map<std::string, uint32_t> ms_staProtoMap;

    static bool __ParseRegInfo(cJSON *pstJson, std::vector<CRegisterInfo> &rstaRegInfo);
    static bool __ParseSupportedTypeArray(cJSON *pstJson, std::vector<uint32_t> &rstaSupportedType);
    static bool __ParseHwInfo(cJSON *pstJson, CHwInfo &rstHwInfo);
    static bool __ParseSupportedModule(cJSON *pstJson, std::map<uint32_t, uint32_t> &rstaMap);
    static bool __ParseSupportedDev(cJSON *pstJson, std::map<uint32_t, uint32_t> &rstaMap);
    static bool __ParseSlotHwInfo(cJSON *pstJson, CSlotHwInfo &rstSlotHwInfo);
    static bool __ParseSlotHwInfoArray(cJSON *pstJson, std::vector<CSlotHwInfo> &rstaSlotHwInfo);
    static bool __ParseSlotRegInfo(cJSON *pstJson, CSlotRegInfo &rstSlotRegInfo);
    static bool __ParseSlotRegInfoArray(cJSON *pstJson, std::vector<CSlotRegInfo> &rstaSlotRegInfo);
    static bool __ParseRegInfoArray(cJSON *pstJson, const char *pcRegInfoName, std::vector<std::vector<CRegisterInfo> > &rstaRegInfo);
    static bool __ParseBoardRegInfo(cJSON *pstJson, CBoardRegisterInfo &rstBoardRegInfo);
    static bool __ParseDevInfo(cJSON *pstJson, std::vector<CDevInfo> &rstaDevInfo);
    static bool __ParseModuleInfo(cJSON *pstJson, std::vector<CModuleInfo> &rstaModuleInfo);
    static bool __ParseCardInfo(cJSON *pstJson, std::map<uint32_t, CCardInfo> &rstaCardRegInfo);
};

