/**
 * @file memory.h
 * @author Lucas Vieira (lucas.engen.cc@gmail.com)
 * @brief Memory handling
 * @version 0.1
 * @date 2020-03-17
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef MEMORY_HEADER
#define MEMORY_HEADER

/**
 * @brief Compare two blocks of memory
 * 
 * @param p1 first block
 * @param p2 second block
 * @param count limit
 * @return int 0 if equal 1 otherwise
 */
extern int iron_memcmp(const void *p1, const void *p2, int count);

#endif
