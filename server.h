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
