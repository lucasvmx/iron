/**
 * @file util.c
 * @author Lucas Vieira (lucas.engen.cc@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-03-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "util.h"

char *strlwr(const char *s)
{
    char *tmp = (char*)calloc(sizeof(char), strlen(s) + 1);

    if(!tmp) {
        return NULL;
    }

    unsigned i = 0;

    while(s[i])
    {
        tmp[i] = tolower((unsigned char)s[i]);
        i++;
    }

    return &tmp[0];
}
