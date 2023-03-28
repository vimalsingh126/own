#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_CLIENTS 1000

void handle_client(int client_id) {
    printf("Handling client %d\n", client_id);
    // Add client handling code here
    sleep(5);
    printf("Client %d handled\n", client_id);
}

int main() {
    for (int i = 1; i <= NUM_CLIENTS; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            // Child process code
            handle_client(i);
            _exit(0);
        } else if (pid < 0) {
            perror("fork");
            exit(1);
        }
    }

    // Wait for all child processes to exit
    for (int i = 1; i <= NUM_CLIENTS; i++) {
        int status;
        wait(&status);
    }

    printf("All clients handled\n");
    return 0;
}
********************************
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

#define NUM_CHILDREN 1000

int main() {
    pid_t child_pids[NUM_CHILDREN];
    int i;

    for (i = 0; i < NUM_CHILDREN; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            // Child process code
            printf("Child process %d\n", getpid());
            sleep(5);
            printf("Child process %d exiting\n", getpid());
            _exit(0);
        } else if (pid > 0) {
            // Parent process code
            child_pids[i] = pid;
        } else {
            // Error forking
            perror("fork");
            exit(1);
        }
    }

    // Parent process code
    printf("Parent process\n");

    while (1) {
        int num_terminated = 0;

        for (i = 0; i < NUM_CHILDREN; i++) {
            // Check if child process has terminated
            int status;
            pid_t result = waitpid(child_pids[i], &status, WNOHANG);

            if (result == -1) {
                perror("waitpid");
                exit(1);
            } else if (result == 0) {
                // Child process has not terminated
                printf("Child process %d still running\n", child_pids[i]);
            } else {
                // Child process has terminated
                printf("Child process %d has terminated\n", child_pids[i]);
                num_terminated++;
            }
        }

        if (num_terminated == NUM_CHILDREN) {
            // All child processes have terminated
            break;
        }

        sleep(1);
    }

    return 0;
}
