#include "msg_rcv.h"
#include "string.h"

void didRecieveMessage(char *json)
{
    Message *message;
    if ((message =  messageFromJSON(json)) == NULL)
    {
        fprintf(stderr, "MSG_RCV : messageFromJSON returns nil");
        return;
    }
    
    if (!strcmp(message->text, "request_answer"))
    {
        return;
    }
    
    printMessage(message);
}