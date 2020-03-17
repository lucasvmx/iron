/**
 * @file file_handler.c
 * @author Lucas Vieira (lucas.engen.cc@gmail.com)
 * @brief File handling functions
 * @version 0.1
 * @date 2020-03-14
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include <errno.h>
#include <stdbool.h>
#include <memory.h>
#include <pthread.h>
#include <sys/stat.h>
#include "md5_handler.h"
#include "file_handler.h"
#include "memory_handler.h"

long int passwords = 0;
long int average = 0;
long int tries = 0;
long int time_spent = 0;

void *show_speed(void *p)
{
    long count1, count2;
    (void)p;
    
    while(true)
    {
        count1 = passwords;
        sleep(1);
        count2 = passwords;
        time_spent++;

        printf("Speed: \x1b[32;1m%ld\x1b[0m passwords/sec\r", count2 - count1);
        fflush(stdout);
        average += (count2 - count1);
        tries++;
    }

    return NULL;
}

static inline FILE *open_file_tr(const char *filename)
{
    FILE *fp = NULL;

    fp = fopen(filename, "r");
    if(!fp)
    {
        fprintf(stderr, "Failed to open '%s': %s\n", filename, strerror(errno));
        return NULL;
    }

    return fp;
}

static int process_file(const char *filename, unsigned char *digest_to_search)
{
    int ch;
    int counter = 0;
    char buffer[MAX_PASSWORD_LEN + 1];
    bool found = false;
    pthread_t id;
    FILE *fp = NULL;
    struct stat fileinfo;
    long eof, bytes_readed = 0;

    // Try to open the file for reading
    fp = open_file_tr(filename);

    if(!fp)
        return 1;

    // Initialize memory
    memset(&fileinfo, 0, sizeof(struct stat));

    // Get file information
    fstat(fp->_fileno, &fileinfo);
    
    // Save filesize
    eof = fileinfo.st_size;

    // Display file information to user
    fprintf(stdout, "Filesize: %ld bytes\n", eof);

    // Display progress 
    pthread_create(&id, NULL, show_speed, NULL);

    // Copy line content into a buffer
    while(bytes_readed <= eof)
    {         
        ch = fgetc(fp);

        // Maybe a windows line ending
        if((ch == 0x0d) && !((ch = fgetc(fp)) == 0x0a))
            ungetc(ch, fp);

        // We reached end of line
        if((ch == 0x0a) || (counter == MAX_PASSWORD_LEN - 1)) 
        {
            buffer[counter] = '\0';
    
            // Compare password hash
            unsigned char *hash = md5_digest(buffer, counter);

            if(iron_memcmp(hash, digest_to_search, MD5_LEN) == 0)
            {
                printf("\nPassword FOUND: \x1b[32;1m%s\x1b[0m\n", buffer);
                found = true;
                break;
            }

            // Reset counter
            counter = 0;

            // Reset buffer
            memset(buffer, 0, sizeof(buffer));

            // Increment number of passwords
            passwords++;

            // Go to next iteration
            continue;
        }

        buffer[counter] = ch;
        counter++;
        bytes_readed++;
    }

    // Close file
    fclose(fp);

    // Stop progress thread
    pthread_cancel(id);

    // Show statistics
    printf("\nPasswords tried: %ld\n", passwords);
    printf("Time spent: %ld seconds\n", time_spent);

    if(tries > 0)
        printf("Average speed: %.2f passwords/second\n", (double)(average / tries));

    return found;
}

int crack_password(const char *dict, unsigned char *password_md5)
{
    return process_file(dict, password_md5);
}
