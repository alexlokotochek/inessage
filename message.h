
typedef struct
{
    char *sender;
    char *reciever;
    char *text;
    int time;
} Message;

Message messageFromJSON(char *text);
char *JSONFromMessage(Message message);