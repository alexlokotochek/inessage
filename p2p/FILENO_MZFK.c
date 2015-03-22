//
//  FILENO_MZFK.c
//  
//
//  Created by Danil Tulin on 3/22/15.
//
//

#include "FILENO_MZFK.h"

#include <stdlib.h>

void getString(char *string)
{
    int i = 0;
    for (char c = '\0'; (c=getchar())!='\n' && c != EOF; i++)
        (string = (char *)realloc(string, i+1), *(string + i) = c);
    *(string + i) = '\0';
}