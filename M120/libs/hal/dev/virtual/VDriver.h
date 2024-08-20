#pragma once
#include <string>
#include <map>
#include <jsoncpp/json/json.h>
#include "util_inc.h"

using string = std::string;
using jindex = Json::ArrayIndex;
using mapsa = std::map<string, jindex>;

class CVirtualDriver
{
public:
    CVirtualDriver(void);
    CVirtualDriver(const string &key, const string &path);
    virtual
    ~CVirtualDriver();
    bool SaveCfg(void);
    void UpdateData(void);
    jindex Index(void);

protected:
    Json::Value m_data;
    Json::Value m_cfg;
    string m_key;
    string m_path;

private:
    bool LoadCfg(void);
    static mapsa m_indexs;

};