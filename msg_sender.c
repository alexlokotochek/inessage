#include "msg_sender.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void sendMessage(Message *message)
{
    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in servaddr;
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(message->reciever);
    servaddr.sin_port = htons(8888);
    
    char *json;
    if ((json = JSONFromMessage(message)) == NULL)
    {
        fprintf(stderr, "SERIOUS APP ERROR : JSONFromMessage returned nil\n");
        kill(getpid(), SIGTERM);
    }
    
    sendto(sockfd, json, strlen(json), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    free(json);
    close(sockfd);
}