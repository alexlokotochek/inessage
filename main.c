#import <Foundation/Foundation.h>

#include <stdio.h> //printf
#include <string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
#include <pthread.h>

#include "mysqlAPI.m"

#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

int main(void)
{
    NSAutoreleasePool *myPool = [[NSAutoreleasePool alloc] init];
    SQL *Database = [[SQL alloc] init];
    
    struct sockaddr_in si_me, si_other;
    
    int s, i, slen = sizeof(si_other) , recv_len;
    char buf[BUFLEN];
    
    s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    memset((char *) &si_me, 0, sizeof(si_me));
    
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    
    bind(s , (struct sockaddr*)&si_me, sizeof(si_me) );
    while(1)
    {
        //try to receive some data, this is a blocking call
        recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen);
        NSLog(@"%s",buf);
        
        if (!strncmp(buf, "STATUS",6))
        {
            Hardware HardwareSet;
            char Message[256]; Message[0] = '\0';
            strcat(Message, [[NSString stringWithFormat:@"Temp#%d:%f EndTemp", 1, [Database GetCurrentTemp]] cStringUsingEncoding:[NSString defaultCStringEncoding]]);
            for (int i = 1; i<=4; i++) {
                HardwareSet = [Database StatusHardware:i];
                strcat(Message, [[NSString stringWithFormat:@"Toggle#%d:%d%d%d",HardwareSet.ToggleNumber, HardwareSet.KeepTempSwitcher, HardwareSet.KeepTemp, HardwareSet.ToggleSwitcher] cStringUsingEncoding:[NSString defaultCStringEncoding]]);
            }
            strcat(Message, "End");
            sendto(s, Message, sizeof(Message), 0, (struct sockaddr*) &si_other, slen);
        }
        
        if (!strncmp(buf, "Toggle #", 8))
        {
            Hardware HardwareSet;
            NSString *content = [NSString stringWithFormat:@"%s",buf];
            content = [content substringFromIndex:8];
            content = [content substringToIndex:[content rangeOfString:[NSString stringWithFormat:@"End"]].location];
            HardwareSet.ToggleNumber = [content characterAtIndex:0] - '0';
            HardwareSet.KeepTempSwitcher = [content characterAtIndex:1] - '0';
            HardwareSet.ToggleSwitcher = [content characterAtIndex:[content length]-1] - '0';
            content = [content substringFromIndex:2];
            content = [content substringToIndex:[content length]-1];
            HardwareSet.KeepTemp = [content intValue];
            [Database UpdateHardware:HardwareSet];
        }
        
        //now reply the client with the same data
        //sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
    }
    
    [myPool drain];
    close(s);
    return 0;
}