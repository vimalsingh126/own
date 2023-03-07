#include <stdio.h>
#include <unistd.h>
#include <sched.h>

int main() {
    pid_t pid = getpid();
    cpu_set_t cpuset;

    // Clear the CPU set
    CPU_ZERO(&cpuset);

    // Set the CPU affinity to core 1
    CPU_SET(1, &cpuset);

    // Call sched_setaffinity to set the CPU affinity
    if (sched_setaffinity(pid, sizeof(cpu_set_t), &cpuset) == -1) {
        perror("sched_setaffinity failed");
        return -1;
    }

    // Check the CPU affinity of the process
    cpu_set_t cpuset_check;
    if (sched_getaffinity(pid, sizeof(cpu_set_t), &cpuset_check) == -1) {
        perror("sched_getaffinity failed");
        return -1;
    }

    // Print the CPU affinity of the process
    printf("CPU affinity of process %d: %d\n", pid, CPU_ISSET(1, &cpuset_check));

    // Run some code here...
    // ...

    return 0;
}
