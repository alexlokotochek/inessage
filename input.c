#include "input.h"

#include <stdlib.h>

char *getString()
{
    char *string = (char *)malloc(sizeof(char));
    int i = 0;
    char c;
    for (c = '\0'; (c=getchar())!='\n' && c != EOF; i++)
        (string = (char *)realloc(string, i+1), *(string + i) = c);
    *(string + i) = '\0';
    return string;
}
