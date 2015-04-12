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

void sendBroadcastMessage(char *text)
{
    // Open a socket
    int sd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sd<=0)
    {
        printf("Error: Could not open socket");
        kill(getpid(), SIGTERM);
    }
    
    // Set socket options
    // Enable broadcast
    int broadcastEnable = 1;
    int ret = setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
    if (ret)
    {
        printf("Error: Could not open set socket to broadcast mode");
        close(sd);
        kill(getpid(), SIGTERM);
    }
    
    // Since we don't call bind() here, the system decides on the port for us, which is what we want.
    
    // Configure the port and ip we want to send to
    struct sockaddr_in broadcastAddr; // Make an endpoint
    memset(&broadcastAddr, 0, sizeof broadcastAddr);
    broadcastAddr.sin_family = AF_INET;
    inet_pton(AF_INET, "172.20.10.15", &broadcastAddr.sin_addr); // Set the broadcast IP address
    broadcastAddr.sin_port = htons(8888); // Set port 1900
    
    // Send the broadcast request, ie "Any upnp devices out there?"
    char *request = "M-SEARCH * HTTP/1.1\r\nHOST:239.255.255.250:1900\r\nMAN:\"ssdp:discover\"\r\nST:ssdp:all\r\nMX:1\r\n\r\n";
    ret = sendto(sd, request, strlen(request), 0, (struct sockaddr*)&broadcastAddr, sizeof broadcastAddr);
    if (ret<0)
    {
        printf("Error: Could not open send broadcast");
        close(sd);
        kill(getpid(), SIGTERM);
    }
    
    // Get responses here using recvfrom if you want...
    close(sd);
}