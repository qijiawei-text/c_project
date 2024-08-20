#pragma once
#include "BoardDriver.h"
#include "VDriver.h"

#define DEF_UPGD_BDL_HEAD_VER           (0u)

#define DEF_UPGD_BDL_HEAD_LEN           (1 * DEF_1KB)
#define DEF_UPGD_BDL_HEAD_TAIL_LEN      (128u)
#define DEF_UPGD_BDL_IMG_HEAD_LEN       (sizeof(ST_UPGD_IMG_HEAD))
#define DEF_UPGD_BDL_IMG_MAX            ((DEF_UPGD_BDL_HEAD_LEN - DEF_UPGD_BDL_HEAD_TAIL_LEN) / \
                                         DEF_UPGD_BDL_IMG_HEAD_LEN)

#define DEF_UPGD_BDL_TAG_LEN            (8u)
#define DEF_UPGD_BDL_KEY_LEN            (8u)
#define DEF_UPGD_BDL_VER_LEN            (16u)
#define DEF_UPGD_BDL_PN_LEN             (32u)
#define DEF_UPGD_BDL_SN_LEN             (32u)
#define DEF_UPGD_BDL_TIME_LEN           (16u)

#define DEF_UPGD_IMG_TAG_LEN            (4u)

typedef struct {
    union {
        uint32_t ulImgTag;                                                  /* Alin 4B                                                 */
        char acImgTag[DEF_UPGD_IMG_TAG_LEN];                                /* Image Tag, refer to DEF_UPGD_IMG_TAG                    */
    };

    uint8_t ucImgType;                                                      /* Image Type, 0 for PPC                                   */
    uint8_t ucHitless;                                                      /* Hitless flag                                            */
    uint8_t ucStrgID;
    uint8_t ucImgIdx;                                                       /* Image Index, start from 0                               */

    union {
        uint32_t ulImgVer;                                                  /* Image Version, BCD code, x.yy.zzzz                      */
        struct {
            uint8_t ucImgVerX;                                              /* BCD code, 0 -- 99                                       */
            uint8_t ucImgVerY;                                              /* BCD code, 0 -- 99                                       */
            uint16_t usImgVerZ;                                             /* BCD code, 0 -- 9999                                     */
        };
    };

    union {
        uint8_t aucTime[4];                                                 /* Image time stamp                                        */

        struct {
            uint8_t ucTimeYear;
            uint8_t ucTimeMon;
            uint8_t ucTimeDay;
            uint8_t ucTimeHour;
        };

        struct {
            uint8_t ucModType;                                              /* Module type                                             */
            uint8_t ucSubType;                                              /* Sub type                                                */
            uint8_t ucPartType;                                             /* Part type                                               */
            struct {
                uint8_t b7Resv : 7;
                uint8_t b1Zip : 1;                                          /* Compressed by create bundle tool                        */
            };
        };
    };

    union {
        uint32_t ulBaseAddr;
        uint32_t ulOffset;                                                  /* Offset in the bundle, from bundle header to image body  */
    };

    uint32_t ulImgCrc32;                                                    /* CRC32 of image                                          */
    uint32_t ulImgLen;                                                      /* Length of image body in bundle, excluded append length  */
    uint8_t ucSectCnt;
    uint8_t ucResv;
    uint16_t usImgHeadCrc32;                                                /* CRC32 of image header, low 2 byte                       */

} ST_UPGD_IMG_HEAD;


typedef struct {
    char acBdlPrefix[32];
    char acBdlTag[DEF_UPGD_BDL_TAG_LEN];                                    /* Bundle Tag                                              */
    char acProdType[DEF_UPGD_BDL_KEY_LEN];                                  /* Bundle product type                                     */
    uint32_t ulHeadCrc32;                                                   /* The CRC32 of the bundle header                          */
    uint32_t ulBdlLen;                                                      /* The length of the whole bundle included header          */
    uint16_t usHeadLen;                                                     /* Bundle header length, refer to DEF_UPGD_BDL_HEAD_LEN    */
    uint8_t ucStructVer;                                                    /* This structure version, refer to DEF_UPGD_BDL_HEAD_VER  */
    uint8_t ucImgCnt;                                                       /* Image Count                                             */
    uint32_t ulBodyCrc32;                                                   /* The CRC32 of the bundle body, excluded bundle header    */
    char acBdlVer[DEF_UPGD_BDL_VER_LEN];                                    /* Bundle Version                                          */
    char acPN[DEF_UPGD_BDL_PN_LEN];                                         /* Bundle PN                                               */
    char acSN[DEF_UPGD_BDL_SN_LEN];                                         /* Bundle SN                                               */
    char acBdlTime[DEF_UPGD_BDL_TIME_LEN];                                  /* ASCII, YYYY-MM-DD hh:mm, e.g. 2018-03-09 15:17          */

    ST_UPGD_IMG_HEAD astImgHead[DEF_UPGD_BDL_IMG_MAX];

} ST_UPGD_BDL_HEAD;

class CBoardVirtual : public CBoard, public CVirtualDriver
{
public:
    CBoardVirtual(void);
    virtual
    ~CBoardVirtual(void);

    virtual bool GetChassicMfg(CMfg &rstMfg);
    virtual bool GetSccMfg(CMfg &rstMfg);

    /* upgrade */
    virtual bool GetUpgInfo(uint32_t uiIdx, uint8_t *pucBuf, uint32_t ulLen);
    virtual bool SetUpgInfo(uint32_t uiIdx, uint8_t *pucBuf, uint32_t ulLen);
};

bool getBundleVer(char *pcFileName, ST_UPGD_BDL_HEAD *pstImgHead);