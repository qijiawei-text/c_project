#include "VThermalDriver.h"

#define CONFIG_FILE_PATH    ("/var/vevice/ThermalConfig.json")

CThermalVirtual::CThermalVirtual(void) : CVirtualDriver("thermal", CONFIG_FILE_PATH)
{
}

CThermalVirtual::~CThermalVirtual()
{
}

bool
CThermalVirtual::GetTemp(int32_t &riTemp)
{
    if (m_data.isMember("temperature")) {
        riTemp = m_data["temperature"].asInt();
        return true;
    }
    return false;
}