//
//  server.h
//  p2p
//
//  Created by Danil Tulin on 3/14/15.
//  Copyright (c) 2015 Danil Tulin. All rights reserved.
//

#include <stdio.h>

#define LOG 1
#ifndef DLog
#if !defined(NDEBUG) && LOG
#   define DLog(fmt, ...) printf((fmt), ##__VA_ARGS__);
#else
#   define DLog(...)
#endif
#endif

pid_t launchServer(int port, int fd_write);
