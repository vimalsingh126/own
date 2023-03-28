#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        // Child process code
        printf("Child process\n");
        sleep(5);
        printf("Child process exiting\n");
        _exit(0);
    } else if (pid > 0) {
        // Parent process code
        printf("Parent process\n");
        while (1) {
            // Check if child process has terminated
            int status;
            pid_t result = waitpid(pid, &status, WNOHANG);

            if (result == -1) {
                perror("waitpid");
                exit(1);
            } else if (result == 0) {
                // Child process has not terminated
                printf("Child process still running\n");
                sleep(1);
            } else {
                // Child process has terminated
                printf("Child process has terminated\n");
                break;
            }
        }
    } else {
        // Error forking
        perror("fork");
        exit(1);
    }

    return 0;
}
