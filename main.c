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

void PARENT_SIGTERM_HANDLER(int signal_number)
{
    DLog("\nParent's SIGTERM handler\n");
    kill(CHILD_PID, SIGTERM);
    DLog("Parent terminated\n");
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
    sa.sa_handler = &PARENT_SIGTERM_HANDLER;
    sigaction(SIGINT, &sa, NULL);
    
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &PARENT_SIGTERM_HANDLER;
    sigaction(SIGKILL, &sa, NULL);
    
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &CHILD_HANDLER;
    sigaction(SIGCHLD, &sa, NULL);
    
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    printf("Enter a command : ");
    
    while (1)
    {
        char *buf = getString();
        
        if (strcmp(buf, "write") == 0)
        {
            printf("Enter a ip : ");
            char *ip = getString();
            
            bzero(&servaddr, sizeof(servaddr));
            servaddr.sin_family = AF_INET;
            servaddr.sin_addr.s_addr = inet_addr(ip);
            servaddr.sin_port = htons(8888);
            
            printf("Enter a message : ");
            char *sendline = getString();
            
            Message *msg = (Message *)malloc(sizeof(Message));
            msg->sender = (char *)malloc(20);
            strcat(msg->sender, "192.168.0.105");
            msg->reciever = (char *)malloc(20);
            strcat(msg->reciever, ip);
            msg->text = sendline;
            
            char *json;
            if ((json = JSONFromMessage(msg)) == NULL)
            {
                fprintf(stderr, "SERIOUS APP ERROR : JSONFromMessage returned nil\n");
                kill(getpid(), SIGTERM);
            }
            
            sendto(sockfd, json, strlen(json), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
            
            releaseMessage(msg);
            free(json);
            
            printf("Enter a command : ");
        }
        
        if (strcmp(buf, "read") == 0)
        {
            if (IS_MSGRCV)
            {
                char rcvm[512];
                read(pipe_fd[0], rcvm, 512);
                
                Message *rcvmsg;
                if ((rcvmsg = messageFromJSON(rcvm)) == NULL)
                {
                    fprintf(stderr, "SERIOUS APP ERROR : messageFromJSON returned nil\n");
                    kill(getpid(), SIGTERM);
                }
                
                printMessage(rcvmsg);
                
                free(rcvmsg);
            }
            else
            {
                printf("No messages\n");
            }
            printf("Enter a command : ");
        }
        
        if (strcmp(buf, "exit") == 0)
        {
            kill(getpid(), SIGINT);
        }
        free(buf);
    }
    
    return 0;
}
