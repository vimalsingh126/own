#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>

#define MAX_INPUT_SIZE 1024

int main() {
    char input[MAX_INPUT_SIZE];
    char md5string[MD5_DIGEST_LENGTH * 2 + 1];  // extra byte for null character
    unsigned char md5sum[MD5_DIGEST_LENGTH];
    int i, n;

    // Read input from keyboard
    printf("Enter input: ");
    fgets(input, MAX_INPUT_SIZE, stdin);
    n = strlen(input);
    if (input[n-1] == '\n') {
        input[n-1] = '\0';
        n--;
    }

    // Compute MD5 hash
    MD5((const unsigned char*)input, n, md5sum);

    // Convert MD5 hash to string
    for (i = 0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf(&md5string[i*2], "%02x", (unsigned int)md5sum[i]);
    }

    // Print MD5 hash as string
    printf("MD5 hash: %s\n", md5string);

    return 0;
}
