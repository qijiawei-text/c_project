/* 这是一个测试历程
    功能：测试系统大端存储还是小端存储*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

int main(int argc, char const* argv[])
{
    //网络赋值
    unsigned short local_num = 0x1234, network_num = 0;
    //将主机字节序列转为网络字节序列
    network_num = htons(local_num);
    printf("将主机字节序列转：0x%hX 化为网络字节序列:0x%hX\n",local_num, network_num);


    //将网络字节序列转为主机字节序列
    local_num = ntohs(network_num);
    printf("将网络字节序列转：0x%hX 化为主机字节序列:0x%hX\n",network_num, local_num);

    int ipv4 = 123456789;
    char* ipv4_str[33];
    ipv4_str = inte_ntop(AF_INET, )
    exit(0);
}