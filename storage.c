#include <string.h>
#include <stdio.h>
#include "message.c"
#include "message.h"
#include "hash.h"
#include "hash.c"

struct Table* hashTable;

void initStorage()
{
    hashTable = createTable(16);
}

void save_json(char* json)
{
    json_error_t error;
    json_t *data = json_loads(json, 0, &error);
    hashTable = insert(json, (void*)data, hashTable);
}

json_t** readAllStorage()
{
    json_t** answer = (json_t**)malloc(sizeof(json_t*) * (hashTable->numberOfElements));
    int counter = 0;
    for (int i = 0; i < hashTable->numberOfCells; ++i)
    {
        if (hashTable->cell[i] != NULL)
        {
            struct ListNode* currentNode = hashTable->cell[i];
            //add this to ans
            answer[counter] = (json_t*)malloc(sizeof(json_t));
            memcpy(answer[counter], currentNode->data, sizeof(json_t));
            ++counter;
            //end
            while (currentNode->next != NULL)
            {
                currentNode = currentNode->next;
                //add this to ans
                answer[counter] = (json_t*)malloc(sizeof(json_t));
                memcpy(answer[counter], currentNode->data, sizeof(json_t));
                ++counter;
                //end
            }
        }
    }
    
    return answer;
}