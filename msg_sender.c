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

// список ip, к которым пользователь прибиндился
char** friendsIP_list = NULL;
int numberOfBinds = 0;

void sendMessage(Message *message)
{
    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in servaddr;
    
    message->sender = getMyIPV4Adress();
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8888);
    char *json;
    if ((json = JSONFromMessage(message)) == NULL)
    {
        fprintf(stderr, "SERIOUS APP ERROR : JSONFromMessage returned nil\n");
        kill(getpid(), SIGTERM);
    }
    /// отправление всем прибинженным адресам
    for (int i = 0; i < numberOfBinds; ++i)
    {
        servaddr.sin_addr.s_addr = inet_addr(friendsIP_list[i]);
        sendto(sockfd, json, strlen(json), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    }
    ///
    free(json);
    close(sockfd);
}

void sendBroadcastMessage(Message *message)
{
    int sd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sd<=0)
    {
        printf("Error: Could not open socket");
        kill(getpid(), SIGTERM);
    }
    
    int broadcastEnable = 1;
    int ret = setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
    if (ret)
    {
        perror("Error: Could not open set socket to broadcast mode");
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
    
    ret = sendto(sd, json, strlen(json), 0, (struct sockaddr*)&broadcastAddr, sizeof broadcastAddr);
    if (ret < 0)
    {
        perror("Error: Could not open send broadcast");
        close(sd);
        kill(getpid(), SIGTERM);
    }
    
    free(json);
    
    close(sd);
}

char **list(int fd)
{
    Message *msg = (Message *)malloc(sizeof(Message));
    msg->text = (char *)calloc(sizeof(char), sizeof("request") + 1);
    *(msg->text) = '\0';
    sprintf(msg->text, "request");
    sendBroadcastMessage(msg);
    
    sleep(3);
    
    char **string = (char **)malloc(sizeof(char *));
    char c = 0;
    size_t stringsqty = 0;
    for (size_t i = 0; (read(fd, &c,  1)) > 0 && c != EOF && c != '\0'; i++)
    {
        if (i == 0)
        {
            string = (char **)realloc(string, (stringsqty + 2) * sizeof(char *));
            string[stringsqty] = (char *)malloc(sizeof(char));
            *string[stringsqty] = '\0';
        }
        
        if (c == '\n')
        {
            i = -1;
            stringsqty++;
        }
        else
        {
            string[stringsqty] = (char *)realloc(string[stringsqty], i + 2);
            string[stringsqty][i] = c;
            string[stringsqty][i+1] = '\0';
        }
    }
    string = (char **)realloc(string, (stringsqty + 1) * sizeof(char *));
    string[stringsqty] = (char *)malloc(sizeof(char));
    string[stringsqty][0] = '\0';
    
    return string;
}