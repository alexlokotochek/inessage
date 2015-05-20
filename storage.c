//
//  storage.c
//  inessage
//
//  Created by Lokotochek on 14.05.15.
//  Copyright (c) 2015 Alexander Laktionov. All rights reserved.
//

#include "storage.h"
#include "message.h"
#include <stdlib.h>
#include <string.h>


struct Table* initializeStorage()
{
    return createTable(4);
}

struct Table* saveMessage_json(char* json, struct Table* storage)
{
    storage = insert(json, NULL, storage);
    return storage;
}

struct Table* saveMessage_msg(Message* msg, struct Table* storage)
{
    char* json = JSONFromMessage(msg);
    storage = insert(json, NULL, storage);
    return storage;
}

void printLOG(struct Table* storage)
{
    struct ListNode* currentNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    for (int i = 0; i < storage->numberOfCells; ++i)
    {
        if (storage->cell[i] != NULL)
        {
            currentNode = storage->cell[i];
            Message* msg = (Message*)malloc(sizeof(Message));
            if (currentNode->key != NULL)
            {
                msg = messageFromJSON(currentNode->key);
                //printf("\n%s\n", currentNode->key);
                if (msg != NULL)
                {
                    printMessage(msg);
                    releaseMessage(msg);
                }else
                {
                    continue;
                }
            }
            
            while (currentNode->next != NULL)
            {
                if (msg == NULL || currentNode->key == NULL || strlen(currentNode->key) == 0)
                    break;
                currentNode = currentNode->next;
                Message* msg = messageFromJSON(currentNode->key);
                if (msg != NULL)
                {
                    //printf("\n%s\n", currentNode->key);
                    printMessage(msg);
                    releaseMessage(msg);
                }else
                {
                    continue;
                }
            }
        }
    }
}

int isContainMessage(Message* msg, struct Table* storage)
{
    char* json = JSONFromMessage(msg);
    return contains(json, storage);
}

int isContainJson(char* json, struct Table* storage)
{
    return contains(json, storage);
}
