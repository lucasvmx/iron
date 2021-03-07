
#ifndef SHA1_HEADER
#define SHA1_HEADER

#include <openssl/sha.h>

/**
 * @brief 
 * 
 * @param buffer 
 * @param count 
 * @return unsigned char* 
 */
extern unsigned char *sha1_digest(unsigned char *buffer, size_t count);

#endif
