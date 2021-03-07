
#include <stdbool.h>
#include <strings.h>
#include "sha1.h"
#include "logger.h"

SHA_CTX ctx;
static bool initialized = false;
unsigned char sha_digest[SHA_DIGEST_LENGTH];

static inline void sha1_begin()
{
    if(!initialized) {
        if(!SHA1_Init(&ctx)) {
            panic("Failed to initialize SHA1 context");
        }
        
        bzero(sha_digest, sizeof(sha_digest));
        initialized = true;
    }
}

static inline void sha1_cleanup()
{
    SHA1_Final(sha_digest, &ctx);
    initialized = false;
}

static inline void sha1_calculate(unsigned char *data, size_t len)
{
    sha1_begin();
    SHA1_Update(&ctx, data, len);
    sha1_cleanup();
}

unsigned char *sha1_digest(unsigned char *buffer, size_t count)
{
    sha1_calculate(buffer, count);
    return &sha_digest[0];
}
