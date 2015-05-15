#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "message.h"

#include "jansson.h"

static char SENDER_KEY[] = "sender";
static char RECIEVER_KEY[] = "reciever";
static char LAST_SENDER_KEY[] = "last_sender";
static char TEXT_KEY[] = "text";
static char TIME_KEY[] = "time";

void releaseMessage(Message *msg)
{
    if (msg == NULL)
    {
        fprintf(stderr, "releaseMessage : Can't handle nil msg\n");
        return;
    }
    
    if (msg->sender == NULL)
    {
        fprintf(stderr, "SERIOUS APP ERROR : releaseMessage : Can't handle nil msg->sender\n");
    }
    if (msg->sender)
        free(msg->sender);
    
    if (msg->reciever == NULL)
    {
        fprintf(stderr, "SERIOUS APP ERROR : releaseMessage : Can't handle nil msg->reciever\n");
    }
    if (msg->reciever)
        free(msg->reciever);
    
    if (msg->last_sender == NULL)
    {
        fprintf(stderr, "SERIOUS APP ERROR : releaseMessage : Can't handle nil msg->last_sender\n");
    }
    if (msg->last_sender)
        free(msg->last_sender);
    
    if (msg->text == NULL)
    {
        fprintf(stderr, "SERIOUS APP ERROR : releaseMessage : Can't handle nil msg->text\n");
    }
    if (msg->text)
        free(msg->text);
    
    free(msg);
}

Message *messageFromJSON(char *json)
{
    if (json == NULL)
    {
        fprintf(stderr, "messageFromJSON : Can't handle nil json parameter\n");
        return NULL;
    }
    
    json_error_t error;
    
    char* fixJson = strstr(json, "}");
    *(fixJson + 1) = '\0';
    
    json_t *data = json_loads(json, 0, &error);
    if(!data)
    {
        fprintf(stderr, "JSON error: on line %d: %s\n", error.line, error.text);
        return NULL;
    }
    
    json_t *sender, *reciever, *last_sender, *text, *time;
    
    sender = json_object_get(data, SENDER_KEY);
    if(!json_is_string(sender))
    {
        fprintf(stderr, "JSON error: sender is not a string\n");
        json_decref(data);
        return NULL;
    }
    
    reciever = json_object_get(data, RECIEVER_KEY);
    if(!json_is_string(reciever))
    {
        fprintf(stderr, "JSON error: reciever is not a string\n");
        json_decref(data);
        return NULL;
    }
    
    
    last_sender = json_object_get(data, LAST_SENDER_KEY);
    if(!json_is_string(last_sender))
    {
        fprintf(stderr, "JSON error: last_sender is not a string\n");
        json_decref(data);
        return NULL;
    }

    
    text = json_object_get(data, TEXT_KEY);
    if(!json_is_string(text))
    {
        fprintf(stderr, "JSON error: text is not a string\n");
        json_decref(data);
        return NULL;
    }
    
    time = json_object_get(data, TIME_KEY);
    if(!json_is_number(time))
    {
        fprintf(stderr, "JSON error: time is not a number\n");
        json_decref(data);
        return NULL;
    }
    
    Message *msg = (Message *)malloc(sizeof(Message));
    
    msg->sender = (char *)malloc(strlen(json_string_value(sender)));
    strcpy(msg->sender, json_string_value(sender));
    
    msg->reciever = (char *)malloc(strlen(json_string_value(reciever)));
    strcpy(msg->reciever, json_string_value(reciever));
    
    msg->last_sender = (char *)malloc(strlen(json_string_value(last_sender)));
    strcpy(msg->last_sender, json_string_value(last_sender));
    
    msg->text = (char *)malloc(strlen(json_string_value(text)));
    strcpy(msg->text, json_string_value(text));
           
    msg->time = (int)json_integer_value(time);
    
    json_decref(data);
    return msg;
}

char *JSONFromMessage(Message *msg)
{
    if (msg == NULL)
    {
        fprintf(stderr, "JSONFromMessage : Can't handle nil message\n");
        return NULL;
    }
    
    
    char *json = (char *)malloc(1024);
    memset(json, 0, 1024);
    
    sprintf(json, "{\"%s\":\"%s\",\"%s\":\"%s\",\"%s\":\"%s\",\"%s\":\"%s\",\"%s\":%ld}", SENDER_KEY, msg->sender, RECIEVER_KEY, msg->reciever, LAST_SENDER_KEY, msg->last_sender, TEXT_KEY, msg->text, TIME_KEY, msg->time);
    
    if (strlen(json) == 1024)
    {
        fprintf(stderr, "JSONFromMessage : JSON string is out of bounds\n");
        return NULL;
    }
    json = (char *)realloc(json, strlen(json) + 1);
    
    Message *checker = messageFromJSON(json);
    if (checker == NULL)
    {
        fprintf(stderr, "messageFromJSON returns non suitable");
        return NULL;
    }
    releaseMessage(checker);
    
    return json;
}

void printMessage(Message *msg)
{
    if (msg)
    {
        printf("FROM : %s\nTO : %s %zu\nLAST_SENDER : %s\nTEXT : %s\nWHEN : %s\n", msg->sender, msg->reciever, strlen(msg->reciever), msg->last_sender, msg->text, ctime(&(msg->time)));
    }
}
