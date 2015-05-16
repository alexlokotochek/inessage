#ifndef ____FILENO_MZFK__
#define ____FILENO_MZFK__

#include <stdio.h>

#define LOG 1
#ifndef DLog
#if !defined(NDEBUG) && LOG
#   define DLog(fmt, ...) printf((fmt), ##__VA_ARGS__);
#else
#   define DLog(...)
#endif
#endif

char *getString();

#endif /* defined(____FILENO_MZFK__) */
