#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUFLEN 512  //Max length of buffer
#define PORT 8000   //The port on which to listen for incoming data

int main(void)
{
    
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
        
        printf("%s\n", buf);
        
        //now reply the client with the same data
        //sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
    }
    
    close(s);
    return 0;
}
