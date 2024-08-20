/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      EndToken.h
   Author:        Zhu Weian
   Date:          2021-02-12
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu Weian] [2021-02-12] [1.0] [Creator]


*****************************************************************************************************/
class CEndToken
{
public:
    virtual bool Find(uint8_t *pucBuf, uint32_t uiLen) = 0;
};

class CMolexEndToken : public CEndToken
{
public:

    bool
    Find(uint8_t *pucBuf, uint32_t uiLen)
    {
        if (nullptr != strstr((char *)pucBuf, ">")) {
            LOG_WRITE(EN_LOG_TRACE, "find >");
            return true;
        }
        return false;

    }
};

class CEotEndToken : public CEndToken
{
public:

    bool
    Find(uint8_t *pucBuf, uint32_t uiLen)
    {
        if (uiLen == 1) {
            if (pucBuf[0] == 0x06) {
                return true;
            }
        }

        if (uiLen >= 2) {
            if (pucBuf[0] == 0x06 || pucBuf[1] == 0x06) {
                return true;
            }
        }

        return false;
    }
};

class COtdrEndToken : public CEndToken
{
public:

    bool
    Find(uint8_t *pucBuf, uint32_t uiLen)
    {

        for (auto staEndToken : m_staEndToken) {

            if (nullptr != strstr((char *)pucBuf, staEndToken.c_str())) {
                return true;
            }
        }

        return false;
    }

private:
    std::vector<std::string> m_staEndToken =
    {"ERR0", "ERR20", "ERR21", "ERR22", "ERR40", "ERR41", "ERR42", "ERR43"};
};

class CWssEndToken : public CEndToken
{
public:
    bool
    Find(uint8_t *pucBuf, uint32_t uiLen)
    {
        if (uiLen < 2) {
            return false;
        }

        if (pucBuf[uiLen - 2] == 0xdd && pucBuf[uiLen - 1] == 0x02) {
            return true;
        }
        return false;
    }


};

