//
//  storage.h
//  inessage
//
//  Created by Lokotochek on 14.05.15.
//  Copyright (c) 2015 Alexander Laktionov. All rights reserved.
//

#ifndef __inessage__storage__
#define __inessage__storage__

#include <stdio.h>
#include "hash.h"
#include "message.h"

struct Table* initializeStorage();

void saveMessage_json(char* json, struct Table* storage);

void printLOG(struct Table* storage);

void saveMessage_msg(Message* msg, struct Table* storage);

int isContainMessage(Message* msg, struct Table* storage);

#endif /* defined(__inessage__storage__) */
