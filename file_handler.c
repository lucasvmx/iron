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
#include "md5_handler.h"
#include "file_handler.h"

volatile long int passwords = 0;

void *show_speed(void *p)
{
#ifdef BENCHMARK
    int speeds[30];
    int i = 0;
    int average = 0;
#endif
    long count1, count2;
    (void)p;
    
    while(true)
    {
        count1 = passwords;
        sleep(1);
        count2 = passwords;

        printf("Speed: \x1b[32;1m%ld\x1b[0m passwords/sec\r", count2 - count1);
        fflush(stdout);
#ifdef BENCHMARK
        speeds[i] = count2 - count1;
        average += speeds[i];
        i++;

        if(i == 30) {
            printf("Average speed: %d\n\n", average / 30);
            i = 0;
        }
#endif
    }

    return NULL;
}

static FILE *open_file_tr(const char *filename)
{
    return fopen(filename, "r");
}

static int process_file(const char *filename, unsigned char *digest_to_search)
{
    int ch;
    int counter = 0;
    char buffer[MAX_PASSWORD_LEN + 1];
    long eof;
    bool found = false;
    pthread_t id;
    long bytes_readed = 0;
    FILE *fp = NULL;

    // Try to open the file for reading
    fp = open_file_tr(filename);
    
    if(!fp)
    {
        printf("Failed to open the file \"%s\": %d (%s)\n", filename, errno, strerror(errno));
        return 1;
    }
    
    // Calculate EOF position
    fseek(fp, 0, SEEK_END);
    eof = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Display progress 
    pthread_create(&id, NULL, show_speed, NULL);

    // Copy line content into a buffer
    do
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

            if(memcmp(hash, digest_to_search, MD5_LEN) == 0)
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
    } while(bytes_readed != eof);

    // Close file
    fclose(fp);

    // Stop progress thread
    pthread_cancel(id);

    printf("\nPasswords tried: %ld\n", passwords);

    return found;
}

int crack_password(const char *dict, unsigned char *password_md5)
{
    return process_file(dict, password_md5);
}
