#include "VDriver.h"
#include <fstream>

mapsa CVirtualDriver::m_indexs;

CVirtualDriver::CVirtualDriver(void)
{
}

CVirtualDriver::CVirtualDriver(const string &key, const string &path) :
    m_key(key),
    m_path(path)
{
    LoadCfg();
}

CVirtualDriver::~CVirtualDriver()
{
}

bool
CVirtualDriver::LoadCfg(void)
{
    std::ifstream is(m_path, std::ifstream::binary);
    bool rc = is.is_open();
    if (!rc) {
        LOG_WRITE(EN_LOG_ERROR, "Failed to open the config file: %s", m_path.data());
        return rc;
    }

    Json::Reader reader;
    rc = reader.parse(is, m_cfg);
    if (!rc) {
        LOG_WRITE(EN_LOG_ERROR, "Failed to parse the config file: %s", m_path.data());
    }

    is.close();

    if (rc && m_cfg.isMember(m_key)) {
        jindex index = Index();
        if (index >= m_cfg[m_key].size()) {
            index = 0;
        }
        m_data = m_cfg[m_key][index];
        return true;
    }

    return false;
}

bool
CVirtualDriver::SaveCfg(void)
{
    Json::StyledWriter sw;
    std::ofstream os(m_path, std::ifstream::binary);
    if (!os.is_open()) {
        LOG_WRITE(EN_LOG_ERROR, "Failed to open the config file: %s", m_path.data());
        return false;
    }
    os << sw.write(m_cfg);
    os.close();
    return true;
}

jindex
CVirtualDriver::Index(void)
{
    if (m_indexs.find(m_key) == m_indexs.end()) {
        m_indexs[m_key] = 0;
        return 0;
    }

    jindex index = m_indexs[m_key];
    m_indexs[m_key] = ++index;
    return index;
}

void
CVirtualDriver::UpdateData(void)
{
    jindex index = Index();
    if (index >= m_cfg[m_key].size()) {
        index = 0;
    }
    m_cfg[m_key][index] = m_data;
}