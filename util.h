/**
 * @file util.h
 * @author Lucas Vieira (lucas.engen.cc@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-03-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef UTIL_HEADER
#define UTIL_HEADER

#define len(x) (sizeof(x)/sizeof(x[0]))

/**
 * @brief Convert a string to lowercase
 * 
 * @param s String
 * @return const char* String lowercase
 */
char *strlwr(const char *s);


#endif
