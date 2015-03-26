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
#include <fcntl.h>

#include "input.h"
#include "server.h"
#include "message.h"

#define PORT 8888

sig_atomic_t IS_MSGRCV = 0;

void SERVER_HANDLER(int signalNumber)
{
    IS_MSGRCV = 1;
}

sig_atomic_t CHILD_PID = 0;

void SIGINT_HANDLER(int signal_number)
{
    kill(getpid(), SIGTERM);
}

void PARENT_SIGTERM_HANDLER(int signal_number)
{
    printf("\nParent's SIGTERM handler\n");
    kill(CHILD_PID, SIGTERM);
    printf("Parent terminated\n");
    exit(0);
}

void CHILD_HANDLER(int signal_number)
{
    wait(0);
}

int main(int argc, char **argv)
{
    int pipe_fd[2];
    pipe(pipe_fd);
    
    pid_t serverPid = launchServer(PORT, pipe_fd[1]);

    CHILD_PID = serverPid;

    struct sigaction sa;
    memset (&sa, 0, sizeof(sa));
    sa.sa_handler = &SERVER_HANDLER;
    sigaction(SIGUSR1, &sa, NULL);
    
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &SIGINT_HANDLER;
    sigaction(SIGINT, &sa, NULL);
    
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &PARENT_SIGTERM_HANDLER;
    sigaction(SIGTERM, &sa, NULL);
    
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &CHILD_HANDLER;
    sigaction(SIGCHLD, &sa, NULL);
    
    
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    

    while (1)
    {
        printf("Enter a command : ");
        char *buf = getString(STDIN_FILENO);
        
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
            
            Message message;
            message.sender = (char *)malloc(20);
            strcat(message.sender, "192.168.0.105");
            message.reciever = (char *)malloc(20);
            strcat(message.reciever, "192.177.0.102");
            message.text = sendline;
            
            char *json;
            json = JSONFromMessage(message);
            
            sendto(sockfd, json, strlen(json), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
            
            free(sendline);
            free(ip);
        }
        
        if (strcmp(buf, "read") == 0)
        {
            if (IS_MSGRCV)
            {
                char rcvm[512];
                read(pipe_fd[0], rcvm, 512);
                
                Message rcvMessage = messageFromJSON(rcvm);
                printf("%s\n", rcvMessage.text);
                printf("FROM : %s\n", rcvMessage.sender);
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
