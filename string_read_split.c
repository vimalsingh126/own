#include <stdio.h>

int main() {
    char data[] = "hello world\n1234 56.78";
    char str[100];
    int num1;
    float num2;

    // Read the first string
    sscanf(data, "%s", str);
    printf("String 1: %s\n", str);

    // Read the second string
    sscanf(data, "%*s %s", str);
    printf("String 2: %s\n", str);

    // Read the integer
    sscanf(data, "%*s %*s %d", &num1);
    printf("Integer: %d\n", num1);

    // Read the float
    sscanf(data, "%*s %*s %*d %f", &num2);
    printf("Float: %.2f\n", num2);

    return 0;
}
