#include <stdlib.h>
#include <string.h>

// Function to securely free a pointer
void secure_free(void **ptr, size_t size) {
    if (ptr != NULL && *ptr != NULL) {
        // Overwrite the contents of the pointer with random garbage
        memset(*ptr, 0, size);
        
        // Free the pointer
        free(*ptr);
        
        // Set the pointer to NULL
        *ptr = NULL;
    }
}
