/**
 * @file file_handler.c
 * @author your name (you@domain.com)
 * @brief 
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

volatile int passwords = 0;

void *show_speed(void *p)
{
    int r1, r2;
#ifdef BENCHMARK
    int speeds[30];
    int i = 0;
    int avg = 0;
#endif
    (void)p;
    
    while(true)
    {
        r1 = passwords;
        sleep(1);
        r2 = passwords;

        printf("Speed: \x1b[32;1m%d\x1b[0m passwords/sec\r", r2 - r1);
        fflush(stdout);
#ifdef BENCHMARK
        speeds[i] = r2 - r1;
        avg += speeds[i];
        i++;

        if(i == 30) {
            printf("Avg speed: %d\n\n", avg / 30);
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

static bool is_printable(int ch)
{
    return ((ch >= 0x20) && (ch <= 0x7f));
}

static bool is_printable_buffer(char *buffer, int len)
{
    for(int i = 0; i < len; i++)
        if(!is_printable(buffer[i]))
            return false;
    
    return true;
}

static int process_file(const char *filename, unsigned char *digest_to_search)
{
    int ch;
    int counter = 0;
    int line = 1;
    char buffer[MAX_PASSWORD_LEN + 1];
    FILE *fp = NULL;
    long eof;
    bool found = false;
    pthread_t id;

    // Try to open the file for reading
    fp = open_file_tr(filename);
    
    if(!fp)
    {
        printf("Failed to open file '%s': %d (%s)\n", filename, errno, strerror(errno));
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
        if(counter == (MAX_PASSWORD_LEN - 1))
            break;
        
        ch = fgetc(fp);

        // We reached end of line
        if(ch == 0x0a) 
        {
            line++;
            buffer[counter] = '\0';

            if(!is_printable_buffer(buffer, counter))
                printf("Warning: password in line %d can't be printed\n", line);
    
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

            passwords++;

            // Go to next iteration
            continue;
        }

        buffer[counter] = ch;
        counter++;
    } while(ftell(fp) != eof);

    // Close file
    fclose(fp);

    // Stop progress thread
    pthread_cancel(id);

    return found;
}

int crack_password(const char *dict, unsigned char *password_md5)
{
    return process_file(dict, password_md5);
}
