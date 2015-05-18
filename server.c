#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include "server.h"
#include "msg_rcv.h"
#include "storage.h"
#include "message.h"
#include "input.h"

#include "math.h"
sig_atomic_t shouldTerminate = 0;

void SIGTERM_HANDLER(int signal_number)
{
    DLog("Server's SIGTERM handler\n");
    shouldTerminate = 1;
    return;
}


void says(char *text)
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



pid_t launchServer(int port, char **neighbours)
{
    /* Our process ID and Session ID */
    pid_t pid, sid;
    
//    pid_t clientPid = getpid();
    
    /* Fork off the parent process */
    pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Wrong pid\n");
        exit(EXIT_FAILURE);
    }
    /* If we got a good PID, then
     we can exit the parent process. */
    if (pid > 0)
    {
        return pid;
    }
    
    /* Change the file mode mask */
    umask(0);
    
    /* Open any logs here */
    
    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0)
    {
        fprintf(stderr, "Wrong sid\n");
        exit(EXIT_FAILURE);
    }
    
    /* Change the current working directory */
    if ((chdir("/")) < 0)
    {
        fprintf(stderr, "Can't read source dir\n");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in si_me, si_other;
    
    int mySocket;
    socklen_t slen = sizeof(si_other);
    char buf[512];

    if((mySocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        fprintf(stderr, "Can't open a socket\n");
        exit(EXIT_FAILURE);
    }
    
    memset((char *) &si_me, 0, sizeof(si_me));
    
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(port);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    
    struct sigaction sa;
    memset (&sa, 0, sizeof(sa));
    sa.sa_handler = &SIGTERM_HANDLER;
    sigaction(SIGTERM, &sa, NULL);
    
    if(bind(mySocket, (struct sockaddr*)&si_me, sizeof(si_me)) < 0)
    {
        fprintf(stderr, "Can't bind the port\n");
        exit(EXIT_FAILURE);
    }
    
    close(STDIN_FILENO);
//    close(STDOUT_FILENO);
    if (!LOG)
        close(STDERR_FILENO);
    
    struct Table* inputStorage = initializeStorage();
    
    while (1)
    {
        memset(buf, 0, 512);
        //ssize_t n = recvfrom(s, buf, 512, 0, (struct sockaddr *) &si_other, &slen);
        if (recvfrom(mySocket, buf, sizeof(int), 0, (struct sockaddr *) &si_other, &slen) != sizeof(int))
        {
            perror("couldn't recieve msg size");
            exit(EXIT_FAILURE);
        }
        
        size_t msgSize = atoi(buf);
        
        DLog("\nMSG SIZE: %zu\n", msgSize);
        if (msgSize > 512)
        {
            DLog("msgSize > 512\n");
            continue;
        }
        
        memset(buf, 0, 512);
        
        ssize_t recievedSize = 0, currentSize = 0;
        while (currentSize < msgSize)
        {
            recievedSize = recvfrom(mySocket, buf, msgSize - currentSize, 0, (struct sockaddr *) &si_other, &slen);
            if (recievedSize < 0)
            {
                DLog("message wasn't recieved completely");
            }
            currentSize += recievedSize;
        }
        
        DLog("Why you are not exiting?\n");
        
        if (strlen(buf) != msgSize)
        {
            DLog("message wasn't recieved completely");
        }
        
//        if (msgSize < 0)
//        {
//            perror("rcvfrom issue");
//            exit(EXIT_FAILURE);
//        }
        
        if (msgSize < 512)
            buf[msgSize] = '\0';
        else
            buf[511] = '\0';

        DLog("Server got %zdz bytes\n", msgSize);
        DLog("Row data: %s\n", buf);
        
        if (strlen(buf))
        {
            inputStorage = didRecieveMessage(buf, neighbours, inputStorage);
        }
        
        
        if (shouldTerminate)
        {
            DLog("Server terminated\n");
            close(mySocket);
            exit(0);
        }
        
        memset(buf, 0, 512);
        msgSize = 0;
    }
}
