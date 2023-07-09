#include <stdio.h>
#include <sodium.h>

int main(void)
{
    if (sodium_init() < 0) {
        printf("Error initializing libsodium.\n");
        return 1;
    }

    int num=14100654;
    int num2=0;

    while(num--)
    {

      num2=rand();  

    unsigned char random_data[4];  // 4 bytes for a 32-bit random number

    randombytes_buf(random_data, sizeof(random_data));

    // Convert the random bytes to an integer
    unsigned int random_number = *((unsigned int*)random_data);

    printf("%d\n", num2);
    }
    return 0;
}

