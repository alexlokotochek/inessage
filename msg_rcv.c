#include "msg_rcv.h"
#include "string.h"

#include "msg_sender.h"

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void didRecieveMessage(char *json, char **neighbours)
{
    Message *message;
    if ((message =  messageFromJSON(json)) == NULL)
    {
        fprintf(stderr, "MSG_RCV : messageFromJSON returns nil");
        return;
    }
    
    printf("\n");
    printMessage(message);
    releaseMessage(message);
}