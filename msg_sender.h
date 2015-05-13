#ifndef __msg_sender__h
#define __msg_sender__h
#include <stdio.h>
#include "message.h"

char** friendsIP_list = NULL;
int numberOfBinds = 0;

void sendMessage(Message *message);
void sendBroadcastMessage(Message *message);
char **list(int fd);

#endif