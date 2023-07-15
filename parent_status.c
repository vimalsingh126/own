#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t child_pid = fork();

    if (child_pid == 0) {
        // Child process
        sleep(5);  // Simulate some work
        pid_t parent_pid = getppid();
        printf("Child: My parent's PID is %d\n", parent_pid);
        // Monitor parent process status, e.g., using kill() or waitpid()
        // ...
    } else if (child_pid > 0) {
        // Parent process
        printf("Parent: My PID is %d\n", getpid());
        wait(NULL);  // Wait for the child process to finish
        printf("Parent: Child process has finished.\n");
    } else {
        // Error handling
        fprintf(stderr, "Fork failed\n");
        return 1;
    }

    return 0;
}



#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t child_pid = fork();

    if (child_pid == 0) {
        // Child process
        sleep(5);  // Simulate some work
        pid_t parent_pid = getppid();
        printf("Child: My parent's PID is %d\n", parent_pid);

        // Check if parent is running
        if (getpgid(parent_pid) == -1) {
            printf("Parent process is not running. Exiting child process.\n");
            return 0;
        }

        // Continue with child process logic...
        // ...
    } else if (child_pid > 0) {
        // Parent process
        printf("Parent: My PID is %d\n", getpid());
        wait(NULL);  // Wait for the child process to finish
        printf("Parent: Child process has finished.\n");
    } else {
        // Error handling
        fprintf(stderr, "Fork failed\n");
        return 1;
    }

    return 0;
}




#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main() {
    pid_t child_pid = fork();

    if (child_pid == 0) {
        // Child process
        sleep(5);  // Simulate some work
        pid_t parent_pid = getppid();
        printf("Child: My parent's PID is %d\n", parent_pid);

        while (1) {
            // Check if parent is running
            if (getpgid(parent_pid) == -1 && errno == ESRCH) {
                printf("Parent process is not running. Exiting child process.\n");
                return 0;
            }

            sleep(1);  // Sleep for 1 second before checking again
        }

        // Continue with child process logic...
        // ...
    } else if (child_pid > 0) {
        // Parent process
        printf("Parent: My PID is %d\n", getpid());
        wait(NULL);  // Wait for the child process to finish
        printf("Parent: Child process has finished.\n");
    } else {
        // Error handling
        fprintf(stderr, "Fork failed\n");
        return 1;
    }

    return 0;
}
