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

#include <stdio.h>





int mian( )
{
    // int server_socket;
    // struct sockaddr_in server_addr;
    // int addr_len = sizeof(server_addr);
    // char buf[MAXSIZE];
    // int iDataNum;

    // //绑定socket
    // if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    // {
    //     perror("server_socket error");
    //     return 1;
    // }

    // //配置服务器端套接字
    // bzero(&server_addr, sizeof(server_addr));
    // server_addr.sin_family = AF_INET;
    // server_addr.sin_port = htons(SERVER_PORT);
    // // if ( inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr) == -1)
    // // {
    // //     perror("server_socket error");
    // //     return 1;
    // // }
    // server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // printf("server_addr.sin_family: %d", server_addr.sin_family);
    // printf("server_addr.sin_port: %d", server_addr.sin_port);

    // close(server_socket);

    printf("hello word\n");
    return 0;
}



