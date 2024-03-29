#include "msg_rcv.h"
#include "string.h"
#include "msg_sender.h"
#include "local.h"
#include "input.h"

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

void sayss(char *text)
{
    char saying[255] = "say -v Milena ";
    strcat(saying, text);
    if (fork() == 0)
    {
        char* voiceArgs[] = {"/bin/bash", "-c", saying, NULL};
        execv(voiceArgs[0], voiceArgs);
        exit(1);
    }
    while(wait(0) > 0);
}


struct Table* didRecieveMessage(char *json, char **neighbours, struct Table* storage)
{
    Message *message;
    if ((message =  messageFromJSON(json)) == NULL)
    {
        fprintf(stderr, "MSG_RCV : messageFromJSON returns nil");
        return storage;
    }
    
    
    DLog("Server recieved raw data:%s\n", json);
    
    if (isContainMessage(message, storage) || ((time(NULL) - message -> time) >= 5))
    {
        DLog("Decline a message\n");
        printMessage(message);
        return storage;
    }
    
    sayss("Вам пришло сообщение от");
    sayss(message->sender);
    sayss(message->text);
    //sayss(ctime(&(message->time)));
    
    int numberOfNeighbours = 0;
    while (neighbours[numberOfNeighbours] != NULL)
    {
        ++numberOfNeighbours;
    }
    
    char* myIP = getMyIPV4Adress();
    
    if (strcmp(getIPV4BroadcastAdress(), message->reciever) == 0)
    {
        printf("You have a broadcast message:\n");
        printMessage(message);
    }
    else if (strcmp(myIP, message->reciever) != 0)
    {
        DLog("\nRescending a message :\n");
        printMessage(message);
        
        for (int i = 0; i < numberOfNeighbours; ++i)
        {
            if (strcmp(neighbours[i], message->last_sender) != 0)
            {
                DLog("Rescending to %s\n", neighbours[i]);
                // кроме того, от кого оно пришло
                sendMessage(message, neighbours[i]);
            }
        }
    }
    else
    {
        storage = saveMessage_json(json, storage);
        printf("You have a message:\n");
        printMessage(message);
    }

    releaseMessage(message);
    
    return storage;
}