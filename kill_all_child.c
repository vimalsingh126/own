#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        // child process
        sleep(10); // sleep for 10 seconds
        exit(0);
    } else if (pid > 0) {
        // parent process
        sleep(5); // sleep for 5 seconds
        kill(-pid, SIGTERM); // send SIGTERM to the child process and all its descendants
    } else {
        // error handling
        perror("fork");
        exit(1);
    }

    return 0;
}
*************
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int main()
{
    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        sleep(10);
        printf("Child process exiting\n");
        return 0;
    } else {
        // Parent process
        printf("Parent process waiting...\n");

        // Wait for child process to finish
        int status;
        waitpid(pid, &status, 0);

        // Kill all child processes
        pid_t pgid = getpgid(pid);
        kill(-pgid, SIGTERM);

        printf("Parent process exiting\n");
        return 0;
    }
}
********************
To kill a child of a child process in C, you need to get the process ID (PID) of the child process and then send a signal to it. You can do this using the kill system call.

Here's an example code snippet that demonstrates how to kill a child process of a child process:

c
Copy code
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main() {
    pid_t pid1, pid2;
    pid1 = fork();
    if (pid1 == 0) {
        // child process 1
        pid2 = fork();
        if (pid2 == 0) {
            // child process 2
            // do some work
            printf("Child process 2 is running...\n");
            sleep(10);
            printf("Child process 2 is done.\n");
            exit(0);
        }
        else {
            // parent of child process 2
            sleep(5);
            printf("Parent of child process 2 is sending SIGTERM to child process 2...\n");
            kill(pid2, SIGTERM);
            printf("Parent of child process 2 is done.\n");
            exit(0);
        }
    }
    else {
        // parent of child process 1
        sleep(15);
        printf("Parent of child process 1 is sending SIGTERM to child process 1...\n");
        kill(pid1, SIGTERM);
        printf("Parent of child process 1 is done.\n");
        exit(0);
    }
}
In this example, we create two child processes: child process 1 and child process 2. Child process 2 does some work and exits after 10 seconds. Child process 1 waits for 5 seconds and then sends a SIGTERM signal to child process 2. Parent process waits for 15 seconds and then sends a SIGTERM signal to child process 1. When a process receives a SIGTERM signal, it terminates all of its child processes.





********

