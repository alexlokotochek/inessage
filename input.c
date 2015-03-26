//
//  FILENO_MZFK.c
//  
//
//  Created by Danil Tulin on 3/22/15.
//
//

#include "input.h"

#include <stdlib.h>

char *getString(int descriptor)
{
    char *string = (char *)malloc(sizeof(char));
    int i = 0;
    char c;
    for (c = '\0'; (c=getchar())!='\n' && c != EOF; i++)
        (string = (char *)realloc(string, i+1), *(string + i) = c);
    *(string + i) = '\0';
    return string;
}
