#include "msg_sender.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <ifaddrs.h>

#include "local.h"

void sendMessage(Message *message, char *address)
{
    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in servaddr;
    
    message->last_sender = getMyIPV4Adress();
    
    printMessage(message);
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(address);
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

void sendBroadcastMessage(Message *message)
{
    int sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sd<=0)
    {
        printf("Error: Could not open socket");
        kill(getpid(), SIGTERM);
    }
    
    int broadcastEnable = 1;
    int ret = setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
    if (ret)
    {
        printf("Error: Could not open set socket zCXVto broadcast mode");
        close(sd);
        kill(getpid(), SIGTERM);
    }
    
    struct sockaddr_in broadcastAddr;
    memset(&broadcastAddr, 0, sizeof broadcastAddr);
    broadcastAddr.sin_family = AF_INET;
    
    char *IPV4Adress = getMyIPV4Adress();
    char *IPV4BroadcastAdress = getIPV4BroadcastAdress();
    
    message->sender = IPV4Adress;
    message->reciever = IPV4BroadcastAdress;
    
    inet_pton(AF_INET, IPV4BroadcastAdress, &broadcastAddr.sin_addr);
    broadcastAddr.sin_port = htons(8888);
    
    char *json;
    if ((json = JSONFromMessage(message)) == NULL)
    {
        fprintf(stderr, "SERIOUS APP ERROR : JSONFromMessage returned nil\n");
        kill(getpid(), SIGTERM);
    }
    
    if ((sendto(sd, json, strlen(json), 0, (struct sockaddr*)&broadcastAddr, sizeof broadcastAddr)) < 0)
    {
        printf("Error: Could not open send broadcast");
        close(sd);
        kill(getpid(), SIGTERM);
    }
    
    free(json);
    
    close(sd);
}