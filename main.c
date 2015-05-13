#include <signal.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>

#include "msg_sender.h"
#include "input.h"
#include "server.h"
#include "message.h"
#include "storage.h"
#include "jansson.h"
#include "hash.h"

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

    printf("Client's pid : %d\nServer's pid : %d\n", getpid(), serverPid);
    
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
    
    // список ip, к которым пользователь прибиндился, в msg_sender.c
    // создание пустой хэш-таблицы
    init_storage();
    
    printf("Enter a command : ");
    
    while (1)
    {
        char *buf = getString();
        
        if (strcmp(buf, "w") == 0)
        {
            printf("Enter ip : ");
            char *ip = getString();
            
            printf("Enter a message : ");
            char *sendline = getString();
            
            Message *msg = (Message *)malloc(sizeof(Message));
            msg->reciever = (char *)malloc(20);
            strcat(msg->reciever, ip);
            msg->text = sendline;
            
            sendMessage(msg);
            
            releaseMessage(msg);
            printf("Enter a command : ");
        }
        
        if (strcmp(buf, "b") == 0)
        {
            printf("Enter a message : ");
            char *sendline = getString();
            
            Message *msg = (Message *)malloc(sizeof(Message));
            msg->text = sendline;
            
            sendBroadcastMessage(msg);
            
            releaseMessage(msg);
            printf("Enter a command : ");
        }
        
        if (strcmp(buf, "l") == 0)
        {
            char **availableIPs = list(pipe_fd[0]);
            
            /////
            int i = 0;
            while (availableIPs[i][0] != '\0')
            {
                printf("%s\n", availableIPs[i]);
                ++i;
            }
            printf("Enter number of IPs to bind and them separating by newline: \n");
            scanf("%d", &numberOfBinds);
            friendsIP_list = (char**)malloc(numberOfBinds*sizeof(char*));
            char* input = (char*)malloc(64*sizeof(input));
            for (int i = 0; i < numberOfBinds; ++i)
            {
                scanf("%s", input);
                friendsIP_list[i] = (char*)malloc(strlen(input));
                strcpy(friendsIP_list[i], input);
            }
            free(input);
            /////
            
            printf("Enter a command : ");
        }
        
        if (strcmp(buf, "friends")==0)
        {
            for (int i = 0; i < numberOfBinds; ++i){
                printf("\n%s", friendsIP_list[i]);
            }
            printf("Enter a command : ");
        }
        
        if (strcmp(buf, "history")==0)
        {
            json_t** data = readAllStorage();

            Message current;
            json_t *sender, *reciever, *text, *time;
            for (int i = 0; i < hashTable->numberOfElements; ++i)
            {
                sender = json_object_get(data[i], "sender");
                current.sender = (char*)json_string_value(sender);
                
                reciever = json_object_get(data[i], "reciever");
                current.reciever = (char*)json_string_value(reciever);
                
                sender = json_object_get(data[i], "text");
                current.text = (char*)json_string_value(text);
                
                sender = json_object_get(data[i], "time");
                current.time = atoi(json_string_value(time));
                
                printMessage(&current);
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
