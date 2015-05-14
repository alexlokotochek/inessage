#include "local.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <ifaddrs.h>

char *getMyIPV4Adress()
{
    struct ifaddrs *ifAddrStruct;
    struct ifaddrs *ifa;
    void *tmpAddrPtr;
    
    getifaddrs(&ifAddrStruct);
    
    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (!ifa->ifa_addr)
        {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET)
        {
            tmpAddrPtr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char *addressBuffer = (char *)malloc(INET_ADDRSTRLEN);
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            if (!strcmp(ifa->ifa_name, "en0"))
            {
                if (ifAddrStruct != NULL)
                    freeifaddrs(ifAddrStruct);
                return addressBuffer;
            }
        }
    }
    return NULL;
}

char *getIPV4BroadcastAdress()
{
    struct ifaddrs *ifAddrStruct;
    struct ifaddrs *ifa;
    void *tmpAddrPtr;
    
    getifaddrs(&ifAddrStruct);
    
    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET)
        {
            tmpAddrPtr = &((struct sockaddr_in *)ifa->ifa_dstaddr)->sin_addr;
            char *addressBuffer = (char *)malloc(INET_ADDRSTRLEN);
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            if (!strcmp(ifa->ifa_name, "en0"))
            {
                if (ifAddrStruct != NULL)
                    freeifaddrs(ifAddrStruct);
                return addressBuffer;
            }
        }
    }
    return NULL;
}