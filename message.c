#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "message.h"

#include <jansson.h>

static char SENDER_KEY[] = "sender";
static char RECIEVER_KEY[] = "reciever";
static char TEXT_KEY[] = "text";
static char TIME_KEY[] = "time";

Message messageFromJSON(char *text)
{
    Message message;
    
    char *buf = text;
    
    json_t *root = json_loads(text, 0, NULL);
    
    return message;
}

char *JSONFromMessage(Message message)
{
    char *json = (char *)malloc(512);
    *json = '\0';
    strcat(json, "{");
    
    strcat(json, "\"");
    strcat(json, SENDER_KEY);
    strcat(json, "\"");
    strcat(json, ":");
    
    strcat(json, "\"");
    strcat(json, message.sender);
    strcat(json, "\"");
    strcat(json, ",");
    
    strcat(json, "\"");
    strcat(json, RECIEVER_KEY);
    strcat(json, "\"");
    strcat(json, ":");
    
    strcat(json, "\"");
    strcat(json, message.reciever);
    strcat(json, "\"");
    strcat(json, ",");
    
    strcat(json, "\"");
    strcat(json, TEXT_KEY);
    strcat(json, "\"");
    strcat(json, ":");
    
    strcat(json, "\"");
    strcat(json, message.text);
    strcat(json, "\"");
    strcat(json, ",");
    
//    strcat(json, "\"");
//    strcat(json, TIME_KEY);
//    strcat(json, "\"");
//    strcat(json, ":");
//    
//    strcat(json, "\"");
//    strcat(json, message.time);
//    strcat(json, "\"");
//    strcat(json, ",");
    
    strcat(json, "}");
    
    return json;
}
