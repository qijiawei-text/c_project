/*****************************************************************************************************
   Copyright (C) 2012, Oplink Communications Inc.
   FileName:      DevCVData.h
   Author:        Zhu WeiAn
   Date:          2022-10-10
   Version:       1.0
   Description:
   Function List:

   History:
   [Zhu WeiAn] [2022-10-10] [1.0] [Creator]


*****************************************************************************************************/

typedef enum enCvLine {
    CV_LINE_W = 0,
    CV_LINE_P,
    CV_LINE_CNT
} EnCvLine;

typedef struct {
    int16_t sOutputPowerR;  /* R represent Mux-R on panel */
    int16_t sInputPowerR;
    int16_t sOutputPowerT;  /* T represent Mux-T on panel */
    int16_t sInputPowerT;
} CCvMuxStatusData;

typedef struct {
    int16_t sOutputPowerR; /* Client-R on AMP */
    int16_t sInputPowerR;
    int16_t sOutputPowerT; /* Client-T on AMP */
    int16_t sInputPowerT;
} CCvClientStatusData;

typedef struct {
    int16_t sOutputPower;
    int16_t sRefPower;
} CCvRef;