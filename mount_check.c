#include <stdio.h>
#include <stdlib.h>
#include <sys/statfs.h>

int isDirectoryMountedOnTmpfs(const char* directory) {
    struct statfs fs;
    if (statfs(directory, &fs) == -1) {
        perror("statfs failed");
        return -1;
    }

    const unsigned long tmpfsType = 0x01021994;  // Update with the correct f_type value for tmpfs

    if (fs.f_type == tmpfsType) {
        return 1;
    } else {
        return 0;
    }
}

int main() {
    const char* directory = "/mnt/hello";

    int isMounted = isDirectoryMountedOnTmpfs(directory);
    if (isMounted == -1) {
        printf("Failed to check if directory is mounted on tmpfs\n");
        return 1;
    }

    if (isMounted) {
        printf("Directory %s is mounted on tmpfs\n", directory);
    } else {
        printf("Directory %s is not mounted on tmpfs\n", directory);
    }

    return 0;
}
