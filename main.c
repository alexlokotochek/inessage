#include <signal.h>
//
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>

#include "msg_sender.h"
#include "input.h"
#include "server.h"
#include "message.h"
#include "local.h"
#include "storage.h"
#include <time.h>

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

void say(char *text)
{
    char saying[255] = "say -v Yuri ";
    strcat(saying, text);
    if (fork() == 0)
    {
        char* voiceArgs[] = {"/bin/bash", "-c", saying, NULL};
        execv(voiceArgs[0], voiceArgs);
        exit(1);
    }
    while(wait(0) > 0);
}

int main(int argc, char **argv)
{
    int friendsNumber = 0;
    printf("Your IP: %s\n", getMyIPV4Adress());
    say("Введите количество друзей");
    printf("Enter friends number and list of them: ");
    scanf("%d", &friendsNumber);
    char **neighbours = (char **)malloc((friendsNumber + 1) * sizeof(char *));
    
    for(int i = 0; i < friendsNumber; ++i)
    {
        char *ip = malloc(255);
        scanf("%s", ip);
        neighbours[i] = ip;
    }
    neighbours[friendsNumber] = NULL;
    
    pid_t serverPid = launchServer(PORT, neighbours);

    say("Сервер и клиент запущены");
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
    sa.sa_handler = &PARENT_SIGTERM_HANDLER;
    sigaction(SIGSTOP, &sa, NULL);
    
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &CHILD_HANDLER;
    sigaction(SIGCHLD, &sa, NULL);
    
    struct Table* outcomeStorage = initializeStorage();
    
    printf("Enter a command : ");
    
    while (1)
    {
        char *buf = getString();
        
        if (strcmp(buf, "w") == 0)
        {
            printf("Enter a ip : ");
            //say("Введите ай пи");
            char *ip = getString();
            
            printf("Enter a message : ");
            //say("Введите сообщение");
            char *sendline = getString();
            
            Message *msg = (Message *)malloc(sizeof(Message));
            msg->sender = getMyIPV4Adress();
            msg->reciever = (char *)malloc(20);
            msg->last_sender = getMyIPV4Adress();
            //Возможно, cтоит уже здесь инициализировать ластсендера
            strcat(msg->reciever, ip);
            msg->text = sendline;
            msg->time = (long)time(NULL);
            outcomeStorage = saveMessage_msg(msg, outcomeStorage);
            
            for(int i = 0; i < friendsNumber; ++i)
                sendMessage(msg, neighbours[i]);
            
            releaseMessage(msg);
            printf("Enter a command : ");
            //say("Введите команду");
        }
        
        if (strcmp(buf, "b") == 0)
        {
            printf("Enter a message : ");
            say("Введите сообщение");
            
            Message *msg = (Message *)malloc(sizeof(Message));
            msg->text = getString();
            msg->last_sender = getMyIPV4Adress();
            msg->time = time(NULL);
            
            sendBroadcastMessage(msg);
            
            outcomeStorage = saveMessage_msg(msg, outcomeStorage);
            
            releaseMessage(msg);
            printf("Enter a command : ");
            say("Введите команду");
        }
        
        if (strcmp(buf, "log") == 0)
        {
            printLOG(outcomeStorage);
        }
        
        if (strcmp(buf, "exit") == 0)
        {
            say("До свидания!");
            PARENT_SIGTERM_HANDLER(0);
        }
        
        printf("\nEnter a Command : ");
        free(buf);
    }
    
    return 0;
}
