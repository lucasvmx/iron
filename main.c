/**
 * @file main.c
 * @author Lucas Vieira (lucas.engen.cc@gmail.com)
 * @brief Main program
 * @version 0.1
 * @date 2020-03-14
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>
#include "crack.h"
#include "util.h"
#include "logger.h"
#include "main.h"

#define MAX_FILENAME        260
#define MD5_EXAMPLE_HASH    "e3f5c287696333b41bdff72dc48d4054"
#define SHA1_EXAMPLE_HASH   "c22b5f9178342609428d6f51b2c5af4c0bde6a42"

static const struct algorithms alg_list[] = 
{
    { "md5", MD5_DIGEST_LENGTH, ID_MD5 },
    { "sha1", SHA_DIGEST_LENGTH, ID_SHA1 }
};

void usage(char **argv)
{
    printf("\nUsage: %s -h <password md5> -d <dictionary> -t <algorithm>\n\n", argv[0]);
    printf("Supported algorithms:\n");
    printf("- SHA1\n");
    printf("- MD5\n\n");
    printf("Example: %s -h \"%s\" -d \"file.dic\" -t MD5\n", argv[0], MD5_EXAMPLE_HASH);
}

int main(int argc, char **argv)
{
    int opt;
    char *hex_hash = NULL;
    char *hash_copy = NULL;
    char alg_name[5] = {0};
    char dictionary[MAX_FILENAME];
    unsigned char *hash = NULL; 
    bool have_hash = false, have_dict = false, have_alg = false;

    // Parse command line arguments
    while((opt = getopt(argc, argv, ":h:d:t:")) != -1)
    {
        switch(opt)
        {
            case 'h':
                if(optarg != NULL)
                {                    
                    // Copies hash into a buffer
                    hash_copy = optarg;
                    have_hash = true;
                }
                break;

            case 'd':
                if(optarg != NULL) 
                {
                    strncpy(dictionary, optarg, sizeof(dictionary));
                    have_dict = true;
                }
                break;
            
            case 't':

                if(optarg != NULL)
                {
                    // Check algorithm type
                    char *algorithm = strlwr(optarg);
                    size_t alg_str_len = strlen(algorithm);

                    strncpy(alg_name, algorithm, len(alg_name));

                    assert(algorithm != NULL);

                    for(unsigned i = 0; i < len(alg_list); i++) 
                    {
                        if(strncmp(algorithm, alg_list[i].name, alg_str_len) == 0) {
                            have_alg = true;
                            break;
                        }
                    }

                    free(algorithm);
                }

                break;
        }
    }

    // Check if we have parameters
    if(!have_hash || !have_dict || !have_alg)
    {
        usage(argv);
        return 1;
    }

    // Check hash type and perform validations
    size_t hex_hash_len = strlen(hash_copy);
    size_t hash_len = hex_hash_len / 2;
    bool found = false;
    unsigned selected_alg = 0;

    for(unsigned i = 0; i < len(alg_list); i++) 
    {
        if(alg_list[i].hash_len == hash_len) 
        {
            if(strncmp(alg_name, alg_list[i].name, strlen(alg_name)) != 0) {
                print(WARNING, "You have selected %s but it's size is %u", alg_name, hash_len);
                panic("Cannot proceed. Please, review command line options");
            }

            print(INFO, "Algorithm: %s, Hash length: %u", alg_list[i].name, alg_list[i].hash_len);
            selected_alg = i;

            found = true;
            break;
        }
    }

    // No valid hash size
    if(!found) {
        panic("Invalid hash size detected: %u", hash_len);
        return 1;
    }

    // Allocate memory to store hash data
    hex_hash = (char*)calloc(sizeof(char), hex_hash_len);
    hash = (unsigned char*)calloc(sizeof(unsigned char), hash_len);

    if(hex_hash == NULL || hash == NULL)
        panic("Failed to allocate memory: %s", strerror(errno));

    // Copy command-line hex hash into a buffer
    strncpy(hex_hash, hash_copy, hex_hash_len);

    print(INFO, "Searching for password with hash '%s' ...", hex_hash);

    // Convert hash to it's hexadecimal representation
    for(size_t i = 0, j = 0; i < hex_hash_len; i += 2, j++)
    {
        unsigned int value;

        sscanf(&hex_hash[i], "%02x", &value);
        hash[j] = (unsigned char)value;
    }

    print(INFO, "Cracking ...");

    // Try to crack the password
    if(!crack_password(dictionary, hash, alg_list[selected_alg].id))
        print(ERROR, "Password not found!");

    return 0;
}