/**
 * @file md5_handler.c
 * @author Lucas Vieira (lucas.engen.cc@gmail.com)
 * @brief Message digest related functions
 * @version 0.1
 * @date 2020-03-14
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "md5.h"
#include <openssl/md5.h>
#include <stdbool.h>
#include <stdlib.h>
#include <memory.h>

static MD5_CTX context;
static bool initialized = false;
unsigned char digest[MD5_DIGEST_LENGTH + 1];

static inline void md5_begin()
{
    // Initialize only when necessary
    if(!initialized && MD5_Init(&context))
    {
        initialized = true;
        memset(digest, 0, sizeof(digest));
    }
}

static inline void md5_cleanup()
{
    // Copy digest to buffer and cleanup resources
    MD5_Final(digest, &context);
    initialized = false;
}

static inline void md5_calculate(unsigned char *data, unsigned int len)
{
    md5_begin();
    MD5_Update(&context, data, len);
    md5_cleanup();
}

unsigned char *md5_digest(unsigned char *password, unsigned int len)
{
    md5_calculate(password, len);
    digest[MD5_DIGEST_LENGTH] = '\0';

    return &digest[0];
}