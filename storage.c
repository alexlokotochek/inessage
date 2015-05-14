//
//  storage.c
//  inessage
//
//  Created by Lokotochek on 14.05.15.
//  Copyright (c) 2015 Alexander Laktionov. All rights reserved.
//

#include "storage.h"
#include <stdlib.h>


struct Table* initializeStorage()
{
    return createTable(16);
}

void saveMessage_json(char* json, struct Table* storage)
{
    storage = insert(json, NULL, storage);
}

void saveMessage_msg(Message* msg, struct Table* storage)
{
    char* json = JSONFromMessage(msg);
    storage = insert(json, NULL, storage);
}

void printLOG(struct Table* storage)
{
    Message* msg = (Message*)malloc(sizeof(Message));
    for (int i = 0; i < storage->numberOfCells; ++i)
    {
        if (storage->cell[i] != NULL)
        {
            struct ListNode* currentNode = storage->cell[i];
            // current node == char* json
            msg = messageFromJSON(currentNode->key);
            printMessage(msg);
            while (currentNode->next != NULL)
            {
                currentNode = currentNode->next;
                msg = messageFromJSON(currentNode->key);
                printMessage(msg);
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
