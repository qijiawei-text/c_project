#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "b64.h"

static int
__encode(const float fInput)
{
    char *pcEncode = NULL;

    pcEncode = b64_encode((unsigned char *)&fInput, 4);
    if (NULL == pcEncode) {
        printf("Error by b64_encode\n");
        return -1;
    }

    printf("Input: %.2f, encode: %s\n", fInput, pcEncode);

    free(pcEncode);

    return 0;
}

static int
__decode(const char *pcInput)
{
    unsigned char *pucDecode = NULL;
    float fVal = 0.0;

    if (NULL == pcInput) {
        printf("Error by pcInput null\n");
        return -1;
    }

    pucDecode = b64_decode(pcInput, strlen(pcInput));
    if (NULL == pucDecode) {
        printf("Error by b64_decode\n");
        return -1;
    }

    memcpy(&fVal, pucDecode, 4);

    printf("Input: %s, decode: %.2f\n", pcInput, fVal);

    free(pucDecode);

    return 0;
}

int
main()
{

    float fOutputPower = 40.31;
    char *pcOutputPower = "cT0hQg==";

    __encode(fOutputPower);

    __decode(pcOutputPower);

    return 0;
}