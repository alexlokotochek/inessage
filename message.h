#ifndef __message__h
#define __message__h

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