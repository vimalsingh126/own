#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>



void encode_base64(unsigned char *input, size_t input_len, char *output) {
    BIO *bio, *b64;
    BUF_MEM *buffer_ptr;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(bio, input, input_len);
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &buffer_ptr);

    memcpy(output, buffer_ptr->data, buffer_ptr->length);
    output[buffer_ptr->length] = '\0';

    BIO_free_all(bio);
}

int main() {

    //     // Initialize the OpenSSL library
    // if (SSL_library_init() != 1) {
    //     fprintf(stderr, "OpenSSL library initialization failed\n");
    //     return 1;
    // }
    // Generate 32 random bytes
    unsigned char random_bytes[32];
    unsigned char converted[32];
    if (RAND_bytes(random_bytes, sizeof(random_bytes)) != 1) {
        fprintf(stderr, "Failed to generate random bytes\n");
        return 1;
    }

    printf("original  %s\n",random_bytes);   
    encode_base64(random_bytes,sizeof(random_bytes),converted);
    printf("converted  %s\n",converted);


    return 0;
}


