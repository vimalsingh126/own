#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void secure_delete(const char* filename) {
    FILE* fp = fopen(filename, "r+");
    if (fp == NULL) {
        printf("Failed to open file: %s\n", filename);
        return;
    }

    // Get file size
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    // Fill buffer with random data
    srand(time(NULL));
    char* buffer = (char*) malloc(size);
    for (long i = 0; i < size; i++) {
        buffer[i] = (char) rand();
    }

    // Overwrite data multiple times
    for (int i = 0; i < 3; i++) {
        fwrite(buffer, sizeof(char), size, fp);
        fflush(fp);
        fseek(fp, 0, SEEK_SET);
    }

    // Close file and delete it
    fclose(fp);
    remove(filename);
    printf("File %s securely deleted.\n", filename);

    free(buffer);
}

int main() {
    const char* filename = "testfile.txt";
    secure_delete(filename);
    return 0;
}



