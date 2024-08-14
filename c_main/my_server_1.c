// #include <sys/stat.h>
// #include <fcntl.h>
// #include <errno.h>
// #include <netdb.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include <unistd.h>


// #define MAXSIZE 200
// #define SERVER_PORT 6666
// #define SERVER_ADDR "172.16.161.117"

// int main( )
// {
//     int server_socket, client_socket;
//     struct sockaddr_in server_addr, client_addr;
//     int addr_len = sizeof(server_addr);
//     char buf[MAXSIZE];
//     int iDataNum;

//     //绑定socket
//     if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
//     {
//         perror("server_socket error");
//         return 1;
//     }

//     //配置服务器端套接字
//     bzero(&server_addr, sizeof(server_addr));
//     server_addr.sin_family = AF_INET;
//     server_addr.sin_port = htons(SERVER_PORT);
//     if ( inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr) == -1)
//     {
//         perror("server_socket error");
//         return 1;
//     }
//     printf("server_addr.sin_addr: %u\n", server_addr.sin_addr.s_addr);
//     printf("server_addr.sin_port: %hu\n", server_addr.sin_port);
    
//     //bind
//     if(bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
//     {
//         perror("connect");
//         return 1;
//     }
//     //监听
//     if(listen(serverSocket, 5) < 0)
//     {
//         perror("listen");
//         return 1;
//     }
//     while(1)
//     {
//         printf("监听端口： %d\n", SERVER_PORT);
//         client = accept(server_socket, (struct sockaddr*)&client_addr, (socklen_t*)&addr_len);
//         if (client_socket == -1)
//         {
//             perror("accept");
//             continue;
//         }
//         printf("等待消息...\n");
//         printf("IP is %s\n", inet_ntoa(clientAddr.sin_addr));
//         printf("Port is %d\n", htons(clientAddr.sin_port));
//         while(1)

//     }

//     close(server_socket);

//     printf("hello word\n");
//     return 0;
// }


#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


#define SERVER_ADDR "172.16.161.117"
#define SERVER_PORT 6666
 
/*
监听后，一直处于accept阻塞状态，
直到有客户端连接，
当客户端如数quit后，断开与客户端的连接
*/
 
int main()
 
{
 
    //调用socket函数返回的文件描述符
    int serverSocket;
    //声明两个套接字sockaddr_in结构体变量，分别表示客户端和服务器
    struct sockaddr_in server_addr;
    struct sockaddr_in clientAddr;
    int addr_len = sizeof(clientAddr);
    int client;
    char buffer[200];
    int iDataNum;
 
    //连接socke
    if((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        return 1;
    }
    bzero(&server_addr, sizeof(server_addr));
    //初始化服务器端的套接字，并用htons和htonl将端口和地址转成网络字节序
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    //连接ip
    if ( inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr) == -1)
    {
        perror("server_socket error");
        return 1;
    }


    //对于bind，accept之类的函数，里面套接字参数都是需要强制转换成(struct sockaddr *)
    //bind三个参数：服务器端的套接字的文件描述符，
    if(bind(serverSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect");
        return 1;
    }
 
    //设置服务器上的socket为监听状态
    if(listen(serverSocket, 5) < 0)
    {
        perror("listen");
        return 1;
    }
    while(1)
    {
        printf("监听端口: %d\n", SERVER_PORT);
        //调用accept函数后，会进入阻塞状态
        client = accept(serverSocket, (struct sockaddr*)&clientAddr, (socklen_t*)&addr_len);
        if(client < 0)
        {
            perror("accept");
            continue;
        }
        printf("等待消息...\n"); 
        //inet_ntoa ip地址转换函数，将网络字节序IP转换为点分十进制IP
        printf("IP is %s\n", inet_ntoa(clientAddr.sin_addr));
        printf("Port is %d\n", htons(clientAddr.sin_port));
        while(1)
        {
            printf("读取消息:");
            buffer[0] = '\0';
            iDataNum = recv(client, buffer, 1024, 0);
            if(iDataNum < 0)
            {
                perror("recv null");
                continue;   
            }
            buffer[iDataNum] = '\0';
            if(strcmp(buffer, "quit") == 0)
                break;
            printf("%s\n", buffer);
            printf("发送消息:");
            scanf("%s", buffer);
            printf("\n");
            send(client, buffer, strlen(buffer), 0);
            if(strcmp(buffer, "quit") == 0)
            break;
        }   
    }
    close(serverSocket);
    return 0;
 
}