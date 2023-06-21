#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>

#define BUFFER_SIZE 1024

int sha256_file(const char* filename, unsigned char* hash) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        return -1; // Failed to open file
    }

    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    unsigned char buffer[BUFFER_SIZE];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        SHA256_Update(&sha256, buffer, bytesRead);
    }

    SHA256_Final(hash, &sha256);
    fclose(file);

    return 0; // Success
}

int main() {
    const char* filename = "example.txt";
    unsigned char hash[SHA256_DIGEST_LENGTH];

    if (sha256_file(filename, hash) == 0) {
        printf("SHA-256 hash of '%s': ", filename);
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            printf("%02x", hash[i]);
        }
        printf("\n");
    } else {
        printf("Failed to compute SHA-256 hash of '%s'\n", filename);
    }

    return 0;
}




#include <stdlib.h>
#include <string.h>

// Securely delete a pointer
void secure_delete(void* ptr, size_t size) {
    if (ptr == NULL) return;

    volatile char* vptr = (volatile char*)ptr;
    while (size--) {
        *vptr++ = rand() & 0xFF;
    }
    
    free(ptr);
}
