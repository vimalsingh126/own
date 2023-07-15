#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <sys/vfs.h>
#include <string.h>
#include <errno.h>

const char* directory = "/mnt/vimal";

int isDirectoryMountedOnTmpfs() {
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
    // Create the directory if it doesn't exist
    struct stat st;
    if (stat(directory, &st) == -1) {
        if (errno == ENOENT) {
            if (mkdir(directory, 0755) == -1) {
                perror("mkdir failed");
                return 1;
            }
            printf("Directory %s created\n", directory);
        } else {
            perror("stat failed");
            return 1;
        }
    } else {
        printf("Directory %s already exists\n", directory);
    }

    // Check if the directory is mounted on tmpfs
    int isMounted = isDirectoryMountedOnTmpfs();
    if (isMounted == -1) {
        printf("Failed to check if directory is mounted on tmpfs\n");
        return 1;
    }

    if (isMounted) {
        printf("Directory %s is already mounted on tmpfs\n", directory);
    } else {
        // Mount the directory on tmpfs
        if (mount("tmpfs", directory, "tmpfs", 0, "size=1G") == -1) {
            perror("mount failed");
            return 1;
        }
        printf("Directory %s mounted on tmpfs\n", directory);
    }




        // Unmount the tmpfs
    if (umount(directory) != 0) {
        perror("umount failed");
        return 1;
    }

    // Remove the directory
    if (rmdir(directory) != 0) {
        perror("rmdir failed");
        return 1;
    }

    printf("Directory %s unmounted and removed successfully\n", directory);


    return 0;
}
