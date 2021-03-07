/**
 * @file md5_handler.h
 * @author Lucas Vieira (lucas.engen.cc@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-03-14
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef MD5_HEADER
#define MD5_HEADER

#include <openssl/md5.h>

/**
 * @brief Gets the MD5 digest from the string
 * 
 * @param password String to calculate MD5
 * @param len String length
 * @return unsigned char* MD5 digest
 */
extern unsigned char *md5_digest(unsigned char *password, unsigned int len);

#endif