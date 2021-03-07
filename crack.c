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
#include "md5.h"
#include "sha1.h"
#include "crack.h"
#include "memory.h"
#include "logger.h"

// Global mutex
static pthread_mutex_t mutex;

long int passwords = 0;
long int average = 0;
long int tries = 0;

// Time spent (in seconds)
long int time_spent = 0;
long int time_spent_ms = 0;
static bool can_stop = false;

void *show_speed(void *p)
{
    long count1, count2;
    (void)p;
    unsigned short times = 0;

    fprintf(stdout, "\n");

    while(!can_stop)
    {
        long passwords_tried = 0;

        // Lock the read password count
        while(times < 1000)
        {
            pthread_mutex_lock(&mutex);
            count1 = passwords;
            pthread_mutex_unlock(&mutex);
            
            // Wait for 1ms
            time_spent_ms += 1;
            usleep(1000);
            times++;

            
            // Lock again to read password count and update time spent
            pthread_mutex_lock(&mutex);
            count2 = passwords;
            pthread_mutex_unlock(&mutex);
            passwords_tried += count2 - count1;

            if(can_stop)
                break;
        }
        
        // 1 second has been passed
        if(time_spent_ms == 1000)
            time_spent++;

        printf("Speed: \x1b[32;1m%ld\x1b[0m passwords/sec\r", passwords_tried);
        fflush(stdout);
        average += passwords_tried;
        tries++;
        times = 0;
        passwords_tried = 0;
    }

    fprintf(stdout, "\n");

    return NULL;
}

static void stop_progress_thread(pthread_t thread)
{
    int *exit_status;

    print(INFO, "Stopping ...");
    can_stop = true;
    pthread_join(thread, &exit_status);
    print(OK, "Stopped");
}

static inline FILE *open_file_tr(const char *filename)
{
    FILE *fp = NULL;

    fp = fopen(filename, "r");
    if(!fp)
    {
        print(ERROR, "Failed to open '%s': %s", filename, strerror(errno));
        return NULL;
    }

    return fp;
}

static inline int process_file(const char *filename, unsigned char *digest_to_search, enum algorithm_ids alg_id)
{
    int ch;
    int counter = 0;
    char buffer[MAX_PASSWORD_LEN + 1];
    bool found = false;
    FILE *fp = NULL;
    struct stat fileinfo;
    long eof, bytes_readed = 0;
    size_t digest_length = 0;
    pthread_t progress_thread_id;

    switch(alg_id)
    {
        case ID_MD5: digest_length = MD5_DIGEST_LENGTH; break;
        case ID_SHA1: digest_length = SHA_DIGEST_LENGTH; break;
        default: panic("Invalid digest ID: %u", alg_id);
    }

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
    print(INFO, "Filesize: %ld bytes", eof);

    // Initialize mutex
    if(pthread_mutex_init(&mutex, NULL) != 0)
        panic("Failed to create mutex: %s", strerror(errno));

    // Display progress 
    pthread_create(&progress_thread_id, NULL, show_speed, NULL);

    // Copy line content into a buffer
    while((bytes_readed <= eof) && !(found))
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
            unsigned char *hash = NULL;

            if(alg_id == ID_MD5) {
                hash = md5_digest((unsigned char*)buffer, counter);
            } else if(alg_id == ID_SHA1) {
                hash = sha1_digest((unsigned char*)buffer, counter);
            }

            if(iron_memcmp(hash, digest_to_search, digest_length) == 0)
            {
                print(OK, "Password FOUND: %s", buffer);
                found = true;
            }

            // Reset counter
            counter = 0;

            // Reset buffer
            memset(buffer, 0, sizeof(buffer));

            // Increment number of passwords
            pthread_mutex_lock(&mutex);
            passwords++;
            pthread_mutex_unlock(&mutex);

            // Go to next iteration
            continue;
        }

        buffer[counter] = ch;
        counter++;
        bytes_readed++;
    }

    // Close file
    fclose(fp);

    // Stop progress thread and close mutex
    pthread_mutex_destroy(&mutex);
    stop_progress_thread(progress_thread_id);

    // Show statistics
    print(INFO, "Passwords tried: %ld", passwords);
    if(time_spent < 1) {
        print(INFO, "Time spent: %ld millisseconds", time_spent_ms);
    } else {
        print(INFO, "Time spent: %ld seconds (%ld millisseconds)", time_spent, time_spent_ms);
    }

    if(tries > 0)
    {
        if(time_spent > 1)
            print(INFO, "Average speed: %.2f passwords/second", (double)(average / tries));
        else {
            double speed = passwords / (double)(time_spent_ms / 1000.0);
            print(INFO, "Average speed: %.2f passwords/second", speed);
        }
    }

    return found;
}

int crack_password(const char *dict, unsigned char *password_md5, enum algorithm_ids id)
{
    return process_file(dict, password_md5, id);
}
