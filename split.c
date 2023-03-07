#include <stdio.h>
#include <string.h>

int main() {
    char str[] = "hello              world";
    char *last_space = strrchr(str, ' ');
    
    if (last_space != NULL) {
        // Pointer arithmetic to get the second part of the string
        char *second_part = last_space + 1;
        printf("Second part: %s\n", second_part);
    }
    
    return 0;
}

/*
#include <stdio.h>
#include <string.h>

int main() {
    char str[] = "hello              world";
    char *last_space = strrchr(str, ' ');
    
    if (last_space != NULL) {
        // Pointer arithmetic to get the second part of the string
        char *second_part = last_space + 1;
        printf("Second part: %s\n", second_part);
        
        // Copy the second part into a new string
        char new_str[20];
        strcpy(new_str, second_part);
        printf("New string: %s\n", new_str);
    }
    
    return 0;
}
*/
