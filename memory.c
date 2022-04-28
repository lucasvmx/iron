/**
 * @file memory.c
 * @author Lucas Vieira (lucas.engen.cc@gmail.com)
 * @brief Memory handling
 * @version 0.1
 * @date 2020-03-17
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <stdio.h>
#include "memory.h"

inline int iron_memcmp(const void *p1, const void *p2, int count)
{
    unsigned char *a1 = (unsigned char*)p1;
    unsigned char *a2 = (unsigned char*)p2;
    int i = 0;

    // Stops only in the limit
    while(i < count)
    {
        // if it is different here, there is no need to compare the rest
        if(*a1 != *a2) {
            return 1;
        }

        // Inc values
        a1++;
        a2++;
        i++;
    }

    // Both blocks are equal
    return 0;
}
