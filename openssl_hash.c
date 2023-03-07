#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

void sha256_hash(const char *input, char *output) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;

    // Initialize the SHA256 context
    SHA256_Init(&sha256);

    // Update the context with the input data
    SHA256_Update(&sha256, input, strlen(input));

    // Generate the SHA256 hash
    SHA256_Final(hash, &sha256);

    // Convert the hash to a string format
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(output + (i * 2), "%02x", hash[i]);
    }

    // Terminate the string
    output[SHA256_DIGEST_LENGTH * 2] = '\0';
}

int main() {
    char input[100];
    char output[SHA256_DIGEST_LENGTH * 2 + 1];

    // Get the input from the user
    printf("Enter a number to hash: ");
    scanf("%s", input);

    // Compute the SHA256 hash of the input
    sha256_hash(input, output);

    // Print the hash value
    printf("Hash of %s: %s\n", input, output);

    return 0;
}
