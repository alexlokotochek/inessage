#include "msg_rcv.h"
#include "string.h"
#include "msg_sender.h"
#include "local.h"

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
    
    printMessage(message);
    
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
        //debug - выводим, что мы являемся звеном пересылки
        printf("---resending:");
        printMessage(message);
        printf("---end of resending\n");
        //debug end
        
        // пересылаем всем
        for (int i = 0; i < numberOfNeighbours; ++i)
        {
            
            if (strcmp(neighbours[i], message->last_sender) != 0)
            {
                // кроме того, от кого оно пришло
                sendMessage(message, neighbours[i]);
            }
        }
    }
    else
    {
        // сообщение пришло нам
        printf("You have a message:\n");
        printMessage(message);
    }

    
    releaseMessage(message);
}