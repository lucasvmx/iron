
#ifndef MAIN_HEADER
#define MAIN_HEADER

enum algorithm_ids
{
    ID_MD5,
    ID_SHA1
};

struct algorithms
{
    const char *name;
    unsigned int hash_len;
    short id;
};

#endif
