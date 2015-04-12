#ifndef message__c
#define message__c

typedef struct
{
    char *sender;
    char *reciever;
    char *text;
    int time;
} Message;

void releaseMessage(Message *);

Message *messageFromJSON(char *);
char *JSONFromMessage(Message *);
void printMessage(Message *);

#endif