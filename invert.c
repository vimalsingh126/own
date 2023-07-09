#include <stdio.h>

unsigned int flipBit(unsigned int num, int position) {
    unsigned int mask = 1 << position;
    return num ^ mask;
}

int main() {
    unsigned int number = 7;  // Example number
    int bitPosition = 2;      // Position of the bit to flip

    printf("Before flipping bit: %u\n", number);

    number = flipBit(number, bitPosition);

    printf("After flipping bit: %u\n", number);

    return 0;
}
