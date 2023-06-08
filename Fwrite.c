
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    FILE* file = fopen("data.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    const char* data = "Hello, world!";
    size_t dataSize = strlen(data);

    fwrite(data, sizeof(char), dataSize, file);
    fflush(file);  // Ensure data is written to the file stream

    int fileDescriptor = fileno(file);
    fsync(fileDescriptor);  // Ensure data is written to disk

    fclose(file);

    return 0;
}
