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
#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>
#include "file_handler.h"

#define MAX_FILENAME    260
#define EXAMPLE_HASH    "e3f5c287696333b41bdff72dc48d4054"

void usage(char **argv)
{
    printf("\nUsage: %s -h <password md5> -d <dictionary>\n", argv[0]);
    printf("Example: %s -h \"%s\" -d \"file.dic\"\n", argv[0], EXAMPLE_HASH);
}

int main(int argc, char **argv)
{
    int opt;
    char hex_md5[MD5_DIGEST_LENGTH * 2];
    char dictionary[MAX_FILENAME];
    unsigned char md5[MD5_DIGEST_LENGTH]; 
    bool have_md5 = false, have_dict = false;

    // Parse command line arguments
    while((opt = getopt(argc, argv, ":h:d:")) != -1)
    {
        switch(opt)
        {
            case 'h':
                if(optarg != NULL)
                {
                    if(strlen(optarg) != (MD5_DIGEST_LENGTH * 2))
                    {
                        printf("Invalid MD5 hash detected\n");
                        return 1;
                    }

                    strncpy(hex_md5, optarg, sizeof(hex_md5));
                    have_md5 = true;
                }
                break;

            case 'd':
                if(optarg != NULL) 
                {
                    strncpy(dictionary, optarg, sizeof(dictionary));
                    have_dict = true;
                }
                break;
        }
    }

    // Check if we have parameters
    if(!have_md5 || !have_dict)
    {
        usage(argv);
        return 1;
    }

    // Convert hex md5 hash to a default md5 hash
    for(int i = 0, j = 0; i < (MD5_DIGEST_LENGTH * 2); i += 2, j++)
    {
        char hex[3];
        unsigned char num;

        snprintf(hex, sizeof(hex), "%c%c", hex_md5[i], hex_md5[i+1]);
        num = (unsigned char)strtol(hex, NULL, 16);

        md5[j] = (unsigned char)num;
    }

    printf("Cracking ...\n");

    // Try to crack the password
    if(!crack_password(dictionary, md5))
        printf("\nPassword \x1b[31;1mNOT found\x1b[0m\n");

    return 0;
}