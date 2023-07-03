#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

int main() {
    const char* path = "my_directory";

    int result = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    if (result == 0) {
        printf("Directory created successfully.\n");
    } else {
        printf("Failed to create the directory.\n");
    }

    return 0;
}


thread race avoid


#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4
#define FILENAME "output.txt"

pthread_mutex_t mutex;
FILE* file;

void* writeFile(void* threadID) {
    long tid = (long)threadID;

    // Generate content to write
    char content[20];
    snprintf(content, sizeof(content), "Thread %ld\n", tid);

    // Lock the mutex before writing to the file
    pthread_mutex_lock(&mutex);

    // Write to the file
    fprintf(file, "%s", content);

    // Unlock the mutex after writing
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

    // Initialize the mutex
    pthread_mutex_init(&mutex, NULL);

    // Open the file
    file = fopen(FILENAME, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening the file.\n");
        return 1;
    }

    // Create the threads
    for (t = 0; t < NUM_THREADS; t++) {
        rc = pthread_create(&threads[t], NULL, writeFile, (void*)t);
        if (rc) {
            fprintf(stderr, "Error creating thread %ld.\n", t);
            return 1;
        }
    }

    // Wait for all threads to complete
    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    // Close the file
    fclose(file);

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}






another 




#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex;

void specificFunction() {
    // Lock the mutex before using the specific function
    pthread_mutex_lock(&mutex);

    // Perform operations using the specific function
    // ...

    // Unlock the mutex after using the specific function
    pthread_mutex_unlock(&mutex);
}

void* threadFunction(void* arg) {
    // Call the specific function within the thread
    specificFunction();

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[5];
    int i;

    // Initialize the mutex
    pthread_mutex_init(&mutex, NULL);

    // Create threads
    for (i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, threadFunction, NULL);
    }

    // Wait for threads to complete
    for (i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
