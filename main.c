#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Define the number of threads
#define NUM_THREADS 4

int count = 0; // A counter used to track the number of combinations explored so far

// Required by lack of standard function in C.
void substr(char *dest, char *src, int start, int length) {
    memcpy(dest, src + start, length);
    dest[length] = '\0';
}

// This function can crack the kind of password explained above.
// All combinations that are tried are displayed, and when the password is found, # is put at the start of the line.
void crack(char *salt_and_encrypted) {
    int x, y, z; // Loop counters
    char salt[7]; // String used in hashing the password. Need space for \0
    char plain[7]; // The combination of letters currently being checked

    substr(salt, salt_and_encrypted, 0, 6);

    for (x = 'A'; x <= 'Z'; x++) {
        for (y = 'A'; y <= 'Z'; y++) {
            for (z = 0; z <= 99; z++) {
                sprintf(plain, "%c%c%02d", x, y, z);
                char *enc = (char *)crypt(plain, salt);

                if (strcmp(salt_and_encrypted, enc) == 0) {
                    pthread_mutex_lock(&lock); // Lock the mutex before printing
                    printf("#%-8d%s %s\n", count, plain, enc);
                    pthread_mutex_unlock(&lock); // Unlock the mutex
                    return; // Uncomment this line if you want to speed up the running time
                }
                count++;
            }
        }
    }
}

// Thread function
void *crack_thread(void *arg) {
    crack("..."); // Replace the dots with your encrypted password
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS]; // Thread handles
    int i;

    // Initialize a mutex for thread synchronization
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, crack_thread, NULL);
    }

    // Wait for all threads to finish
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("%d solutions explored\n", count);
    return 0;
}
