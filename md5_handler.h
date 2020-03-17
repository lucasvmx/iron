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

#ifndef MD5_HANDLER_H
#define MD5_HANDLER_H

#define MD5_LEN             16
#define MD5_HASH_LEN        32
#define MD5_HEX_HASH_LEN    (MD5_LEN * 2) + 1

extern unsigned char *md5_digest(char *password, unsigned int len);

#endif