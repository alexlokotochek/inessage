//danchik

#ifndef __message__c
#define __message__c

typedef struct
{
    char *sender;
    char *reciever;
    char *text;
    char *last_sender;
    int time;
    
} Message;

void releaseMessage(Message *);

Message *messageFromJSON(char *);
char *JSONFromMessage(Message *);
void printMessage(Message *);

#endif