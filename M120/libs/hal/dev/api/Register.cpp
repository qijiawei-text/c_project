/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      Register.cpp
   Author:        Zhen Zhang
   Date:          2018-06-01
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhen Zhang] [2018-06-01] [1.0] [Creator]


*****************************************************************************************************/

/*=========================include head files====================*/

#include "Register.h"
#include "DevData.h"
#include "ShmOperator.h"

/*=============================variables=========================*/

std::map<uint32_t, std::shared_ptr<CBoardThread> > CBoardRegister::ms_staRegList;
/*
   template<class T>
   std::map<uint32_t, std::shared_ptr<T>> CDevRegister<T>::ms_staRegList;
 */
/*==============================function=========================*/

std::shared_ptr<CRegister>
CRegisterFactory::GetRegister(uint32_t uiType)
{

    switch (uiType) {
    case MODULE_TYPE_OA:
    {
        return std::static_pointer_cast<CRegister>(std::make_shared<COaRegister>());
    }

    case MODULE_TYPE_OCM:
    {
        return std::static_pointer_cast<CRegister>(std::make_shared<COcmRegister>());
    }

    case MODULE_TYPE_WSS:
    {
        return std::static_pointer_cast<CRegister>(std::make_shared<CWssRegister>());
    }

    case MODULE_TYPE_TDCM:
    {
        return std::static_pointer_cast<CRegister>(std::make_shared<CTdcmRegister>());
    }

    case MODULE_TYPE_PAM4:
    {
        return std::static_pointer_cast<CRegister>(std::make_shared<CPam4Register>());
    }

    case MODULE_TYPE_OPS:
    {
        return std::static_pointer_cast<CRegister>(std::make_shared<COpsRegister>());
    }

    case MODULE_TYPE_DCE:
    {
        return std::static_pointer_cast<CRegister>(std::make_shared<CDceRegister>());
    }

    case MODULE_TYPE_OSC:
    {
        return std::static_pointer_cast<CRegister>(std::make_shared<COscRegister>());
    }

    case MODULE_TYPE_OTDR:
    {
        return std::static_pointer_cast<CRegister>(std::make_shared<COtdrRegister>());
    }

    case MODULE_TYPE_MUX:
    {
        return std::static_pointer_cast<CRegister>(std::make_shared<CMuxRegister>());
    }

    case MODULE_TYPE_EXTEND:
    {
        return std::static_pointer_cast<CRegister>(std::make_shared<CExtendRegister>());
    }

    case MODULE_TYPE_OCS:
    {
        return std::static_pointer_cast<CRegister>(std::make_shared<COcsRegister>());
    }

    default:
        break;
    }

    return nullptr;

}

std::shared_ptr<CRegister>
CRegisterFactory::GetRegister(uint32_t uiType, const std::vector<CRegisterInfo> &c_rstaInfo, uint32_t uiDevIdx)
{

    switch (uiType) {
    case MODULE_TYPE_OA:
    {
        return std::static_pointer_cast<CRegister>(std::make_shared<COaRegister>(c_rstaInfo, uiDevIdx));
    }

    case MODULE_TYPE_VOA:
    {
        return std::static_pointer_cast<CRegister>(std::make_shared<COaRegister>(c_rstaInfo, uiDevIdx));
    }

    case MODULE_TYPE_OCM:
    {
        return std::static_pointer_cast<CRegister>(std::make_shared<COcmRegister>(c_rstaInfo, uiDevIdx));
    }

    case MODULE_TYPE_WSS:
    {
        return std::static_pointer_cast<CRegister>(std::make_shared<CWssRegister>(c_rstaInfo, uiDevIdx));
    }

    case MODULE_TYPE_TDCM:
    {
        return std::static_pointer_cast<CRegister>(std::make_shared<CTdcmRegister>(c_rstaInfo, uiDevIdx));
    }

    case MODULE_TYPE_PAM4:
    {
        return std::static_pointer_cast<CRegister>(std::make_shared<CPam4Register>(c_rstaInfo, uiDevIdx));
    }

    case MODULE_TYPE_OPS:
    {
        return std::static_pointer_cast<CRegister>(std::make_shared<COpsRegister>(c_rstaInfo, uiDevIdx));
    }

    case MODULE_TYPE_DCE:
    {
        return std::static_pointer_cast<CRegister>(std::make_shared<CDceRegister>(c_rstaInfo, uiDevIdx));
    }

    case MODULE_TYPE_OSC:
    {
        return std::static_pointer_cast<CRegister>(std::make_shared<COscRegister>(c_rstaInfo, uiDevIdx));
    }

    case MODULE_TYPE_OTDR:
    {
        return std::static_pointer_cast<CRegister>(std::make_shared<COtdrRegister>(c_rstaInfo, uiDevIdx));
    }

    case MODULE_TYPE_MUX:
    {
        return std::static_pointer_cast<CRegister>(std::make_shared<CMuxRegister>(c_rstaInfo, uiDevIdx));
    }

    case MODULE_TYPE_EXTEND:
    {
        return std::static_pointer_cast<CRegister>(std::make_shared<CExtendRegister>(c_rstaInfo, uiDevIdx));
    }

    case MODULE_TYPE_OCS:
    {
        return std::static_pointer_cast<CRegister>(std::make_shared<COcsRegister>(c_rstaInfo, uiDevIdx));
    }

    default:
        break;
    }

    return nullptr;

}


