//
//  main.c
//  p2p
//
//  Created by Danil Tulin on 3/14/15.
//  Copyright (c) 2015 Danil Tulin. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "server.h"
#define PORT 8888

typedef enum
{
    CommandWriteMessage,
    CommandShowMessages,
}
command_t;

int main(void)
{
    int pipe_fd[2];
    pipe(pipe_fd);
    
//    launchServer(PORT, pipe_fd[1]);
    
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    
    while (1)
    {
        printf("Enter a command : ");
        char buf[512];
        scanf("%s", buf);
        
        if (strcmp(buf, "write") == 0)
        {
            printf("Enter a ip : ");
            char ip[30];
            scanf("%s", ip);
            
            bzero(&servaddr,sizeof(servaddr));
            servaddr.sin_family = AF_INET;
            servaddr.sin_addr.s_addr = inet_addr(ip);
            servaddr.sin_port = htons(8888);
            
            char sendline[1000];
            
            printf("Enter a message : ");
            scanf("%s", sendline);
            
            sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
        }
    }
    
    return 0;
}
