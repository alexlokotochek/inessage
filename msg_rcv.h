
#ifndef __msg__rcv__h
#define __msg__rcv__h

#include <stdio.h>
#include "message.h"
#include "storage.h"

#define LOG 1
#ifndef DLog
#if !defined(NDEBUG) && LOG
#   define DLog(fmt, ...) printf((fmt), ##__VA_ARGS__);
#else
#   define DLog(...)
#endif
#endif

void didRecieveMessage(char *, char **neighbours, struct Table* storage);

#endif