#include "msg_rcv.h"
#include "string.h"

#include "msg_sender.h"

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "local.h"
#include "msg_sender.h"
#include "storage.h"

void didRecieveMessage(char *json, int fd)
{
    save_json(json); // save to message storage
    Message *message;
    if ((message =  messageFromJSON(json)) == NULL)
    {
        fprintf(stderr, "MSG_RCV : messageFromJSON returns nil");
        return;
    }
    
    printf("/Users/lokotochek/Documents/AKOS/inessage/input.h\n");
    printMessage(message);
    
    if (!strcmp(message->text, "request_answer"))
    {
        strcat(message->sender, "\n");
        write(fd, message->sender, sizeof(message->sender));
        releaseMessage(message);
        return;
    }
    
    if (!strcmp(message->text, "request"))
    {
        Message *msg = (Message *)malloc(sizeof(Message));
        msg->reciever = message->sender;
        msg->text = (char *)calloc(sizeof("request_answer"), 1);
        strcat(msg->text, "request_answer");
        
        sendMessage(msg);
        releaseMessage(msg);
    }
    
   if (strcmp(message->reciever, getMyIPV4Adress()) != 0
       && doesContainOnStorage(json) == 0)
   {
       sendMessage(message);
   }
    
    releaseMessage(message);
}