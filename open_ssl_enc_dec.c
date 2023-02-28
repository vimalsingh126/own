#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>



void generate_random_bytes(unsigned char *buf, size_t len) {
    RAND_bytes(buf, len);
}

char *encode_base64(unsigned char *data, size_t len) {
    BIO *bio, *b64;
    BUF_MEM *bptr;
    char *b64text;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    BIO_push(b64, bio);
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(b64, data, len);
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);
    b64text = (char *)malloc(bptr->length + 1);
    memcpy(b64text, bptr->data, bptr->length);
    b64text[bptr->length] = '\0';
    BIO_free_all(b64);
    return b64text;
}

unsigned char *decode_base64(char *b64text, size_t *out_len) {
    BIO *bio, *b64;
    unsigned char *buffer = NULL;
    size_t len = strlen(b64text);

    bio = BIO_new_mem_buf(b64text, len);
    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO_push(b64, bio);

    buffer = (unsigned char *)malloc(len * sizeof(unsigned char));
    *out_len = BIO_read(b64, buffer, len);
    BIO_free_all(b64);

    return buffer;
}

int main() {
    unsigned char data[32];
    size_t len = sizeof(data);
    generate_random_bytes(data, len);

    printf("original  %s",data);

    char *b64text = encode_base64(data, len);
    printf("Base64 encoded data: %s\n", b64text);

    size_t out_len;
    unsigned char *decoded_data = decode_base64(b64text, &out_len);

    printf("dec data  %s",decoded_data);

    if (out_len != len || memcmp(decoded_data, data, len) != 0) {
        printf("Error: Decoded data doesn't match original data.\n");
    } else {
        printf("Decoded data matches original data.\n");
    }

    free(b64text);
    free(decoded_data);

    return 0;
}