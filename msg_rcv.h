
#ifndef __msg__rcv__h
#define __msg__rcv__h

#include <stdio.h>
#include "message.h"
#include "storage.h"

struct Table* didRecieveMessage(char *, char **neighbours, struct Table* storage);

#endif