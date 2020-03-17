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

#include "memory.h"

int iron_memcmp(const void *p1, const void *p2, int count)
{
    int *a1 = (int*)p1;
    int *a2 = (int*)p2;
    int i = 0;

    // Stops only in the limit
    while(i < count)
    {
        // if it is different here, there is no need to compare the rest
        if(*a1 != *a2)
            return 1;

        // Inc values
        a1++;
        a2++;
        i++;
    }

    // Both blocks are equal
    return 0;
}
