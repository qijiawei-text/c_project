#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define DEF_READ_PROC_NAME_PATH                  ("/proc/self/exe")





void test_UTIL_GetProcName()
{
    int iRet = -1;
    char acProcPath[128] = {0};
    char pcProcName[32] = {0};
    char *pcName = NULL;
    int ulLen = sizeof(pcProcName);

    if (NULL == pcProcName) {
        printf("pcProcName NULL\n");
    }

    memset(acProcPath, 0x00, 128);
    iRet = readlink(DEF_READ_PROC_NAME_PATH, acProcPath, 128 - 1);

    pcName = strrchr(acProcPath, '/');
    if (NULL == pcName) {
        pcName = acProcPath;
    }
    /* del the first '/'                                                        */
    pcName++;

    memcpy(pcProcName, pcName, (ulLen > strlen(pcName)) ? strlen(pcName) : ulLen);

    printf("pcProcName : %s\n", pcProcName);
    return 0;
}


int main()
{
    printf("+++++++++++ test UTIL_GetProcName +++++++++++++++\n");
    test_UTIL_GetProcName();
    
    return 0;
}