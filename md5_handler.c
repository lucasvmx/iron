/**
 * @file md5_handler.c
 * @author Lucas Vieira (lucas.engen.cc@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-03-14
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "md5_handler.h"
#include <openssl/md5.h>
#include <stdbool.h>
#include <stdlib.h>
#include <memory.h>

static MD5_CTX context;
static bool initialized = false;
unsigned char digest[MD5_LEN + 1];

static void md5_begin()
{
    if(!initialized && MD5_Init(&context))
    {
        initialized = true;
        memset(digest, 0, sizeof(digest));
    }
}

static void md5_cleanup()
{
    MD5_Final(digest, &context);
    initialized = false;
}

static void md5_calculate(char *data, unsigned int len)
{
    md5_begin();
    MD5_Update(&context, data, len);
    md5_cleanup();
}

unsigned char *md5_digest(char *password, unsigned int len)
{
    md5_calculate(password, len);
    digest[MD5_LEN] = '\0';

    return &digest[0];
}