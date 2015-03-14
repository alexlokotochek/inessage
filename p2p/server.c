//
//  server.c
//  p2p
//
//  Created by Danil Tulin on 3/14/15.
//  Copyright (c) 2015 Danil Tulin. All rights reserved.
//

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include "server.h"

void launchServer(int port, int fd_write)
{
    /* Our process ID and Session ID */
    pid_t pid, sid;
    
    /* Fork off the parent process */
    pid = fork();
    if (pid < 0)
    {
        DLog("Wrong pid\n");
        exit(EXIT_FAILURE);
    }
    /* If we got a good PID, then
     we can exit the parent process. */
    if (pid > 0)
    {
        return;
    }
    
    /* Change the file mode mask */
    umask(0);
    
    /* Open any logs here */
    
    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0)
    {
        DLog("Wrong sid\n");
        exit(EXIT_FAILURE);
    }
    
    /* Change the current working directory */
    if ((chdir("/")) < 0)
    {
        DLog("Can't read source dir\n");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in si_me, si_other;
    
    int s;
    socklen_t slen = sizeof(si_other);
    char buf[512];

    if((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        DLog("Can't open a socket\n");
    }
    
    memset((char *) &si_me, 0, sizeof(si_me));
    
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(port);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if(bind(s, (struct sockaddr*)&si_me, sizeof(si_me)) < 0)
    {
        DLog("Can't bind the port\n");
    }
    
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    memset(buf, 0, 512);
    
    while (1)
    {
        ssize_t n = recvfrom(s, buf, 512, 0, (struct sockaddr *) &si_other, &slen);
        if (n < 512)
            buf[n] = '\0';
        else
            buf[511] = '\0';
        write(fd_write, buf, strlen(buf));
    }
}
