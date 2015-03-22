//
//  main.c
//  p2p
//
//  Created by Danil Tulin on 3/14/15.
//  Copyright (c) 2015 Danil Tulin. All rights reserved.
//

#include <signal.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "FILENO_MZFK.h"
#include "server.h"

#define PORT 8888

sig_atomic_t IS_MSGRCV = 0;

void serverHandler(int signalNumber)
{
    IS_MSGRCV = 1;
}

sig_atomic_t CHILD_PID = 0;

void SIGINT_HANDLER(int signal_number)
{
    kill(getpid(), SIGTERM);
}

void SIGTERM_HANDLER(int signal_number)
{
    printf("\nParent's SIGTERM handler\n");
    kill(CHILD_PID, SIGTERM);
    wait(0);
    printf("Parent terminated\n");
    exit(0);
}

int main(int argc, char **argv)
{
    int pipe_fd[2];
    pipe(pipe_fd);
    
    pid_t serverPid = launchServer(PORT, pipe_fd[1]);

    CHILD_PID = serverPid;

    struct sigaction sa;
    memset (&sa, 0, sizeof(sa));
    sa.sa_handler = &serverHandler;
    sigaction(SIGUSR1, &sa, NULL);
    
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &SIGINT_HANDLER;
    sigaction(SIGINT, &sa, NULL);
    
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &SIGTERM_HANDLER;
    sigaction(SIGTERM, &sa, NULL);
    
    
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    

    while (1)
    {
        printf("Enter a command : ");
        char *buf = getString();
        
        if (strcmp(buf, "write") == 0)
        {
            printf("Enter a ip : ");
            char *ip = getString();
            
            bzero(&servaddr,sizeof(servaddr));
            servaddr.sin_family = AF_INET;
            servaddr.sin_addr.s_addr = inet_addr(ip);
            servaddr.sin_port = htons(8888);
            
            printf("Enter a message : ");
            char *sendline = getString();
            
            sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
            
            free(sendline);
            free(ip);
        }
        
        if (strcmp(buf, "read") == 0)
        {
            if (IS_MSGRCV)
            {
                char message[512];
                
                read(pipe_fd[0], message, 20);
                printf("%s\n", message);
                IS_MSGRCV = 0;
                
                memset(message, 0, 512);
            }
            else
            {
                printf("No messages \n");
            }
        }
        
        if (strcmp(buf, "exit") == 0)
        {
            kill(getpid(), SIGINT);
        }
        
        free(buf);
    }
    
    return 0;
}
