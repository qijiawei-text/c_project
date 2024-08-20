/******************************************************************************
*
* Copyright(C), 2019, OPLINK Tech.Co.,Ltd
*
* Filename:      util_aes.c
* Description:   aes cbc 256 algorithm
* Author:        Subsystem Dev team
* History:
*                [Author]   [Date]      [Version]    [Description]
*                leiz       2019/05/06  Ver 1.0.0    Initial file.
*
******************************************************************************/
#include "aes.h"
#include "util_aes.h"

static AES_KEY m_stDecryptKey32;
static AES_KEY m_stEncryptKey32;

static const unsigned char m_ucUserkey32[] =
{0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x12,
 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x12, 0x34,
 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x12, 0x34, 0x56};

static unsigned char m_ucIv[MAX_BLOCK_SIZE / 8];
static int m_lAesInitFlag = 0;

void
UTIL_AesCbc256InitKey(void)
{
    if (m_lAesInitFlag == 0) {
        memset(&m_stEncryptKey32, 0x00, sizeof(m_stEncryptKey32));
        memset(&m_stDecryptKey32, 0x00, sizeof(m_stDecryptKey32));
        memset(m_ucIv, 0x00, sizeof(m_ucIv));

        AES_set_encrypt_key(m_ucUserkey32, 256, &m_stEncryptKey32);
        AES_set_decrypt_key(m_ucUserkey32, 256, &m_stDecryptKey32);

        m_lAesInitFlag = 1;
    }

    return;
}

void
UTIL_AesCbc256Encrypt(const uint8_t *pucInBuf, uint32_t ulLen, uint8_t *pucOutBuf)
{
    if (NULL == pucInBuf || NULL == pucOutBuf) {
        goto ERR_LABEL;
    }

    UTIL_AesCbc256InitKey();

    memset(m_ucIv, 0x00, sizeof(m_ucIv));
    AES_cbc256_Encrypt(pucInBuf, pucOutBuf, ulLen, &m_stEncryptKey32, m_ucIv, AES_ENCRYPT);

    return;
ERR_LABEL:
    return;
}

void
UTIL_AesCbc256Decrypt(const uint8_t *pucInBuf, uint32_t ulLen, uint8_t *pucOutBuf)
{
    if (NULL == pucInBuf || NULL == pucOutBuf) {
        goto ERR_LABEL;
    }

    UTIL_AesCbc256InitKey();

    memset(m_ucIv, 0x00, sizeof(m_ucIv));
    AES_cbc256_Encrypt(pucInBuf, pucOutBuf, ulLen, &m_stDecryptKey32, m_ucIv, AES_DECRYPT);

    return;
ERR_LABEL:
    return;
}

