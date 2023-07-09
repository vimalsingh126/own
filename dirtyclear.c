#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

#define PAGE_SIZE 4096 // Page size (can vary depending on the system)

int main() {
    // Allocate a large memory region
    size_t size = 1024 * 1024 * 100; // Allocate 100 MB (adjust as per your requirement)
    void* memory = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    // Modify the memory (make it dirty)
    // ...
    
    // Get the number of pages in the allocated memory region
    size_t num_pages = size / PAGE_SIZE;
    
    // Allocate an array to store the page status (dirty or clean)
    unsigned char* page_status = calloc(num_pages, sizeof(unsigned char));
    if (page_status == NULL) {
        perror("Failed to allocate memory for page status");
        exit(1);
    }
    printf("here\n");
    sleep(1);
    // Use mincore to check the status of each page
    if (mincore(memory, size, page_status) == -1) {
        perror("mincore failed");
        exit(1);
    }
    printf("now hwrw");
   sleep(1);
    // Clean the dirty pages
    for (size_t i = 0; i < num_pages; i++) {
        if (page_status[i] & 1) { // Check if the page is dirty
            void* page_address = (uint8_t*)memory + (i * PAGE_SIZE);
            madvise(page_address, PAGE_SIZE, MADV_DONTNEED);
        }
    }
    
    // Free the memory
    munmap(memory, size);
    
    // Free the page status array
    free(page_status);
    
    return 0;
}

