#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Ana l-child process! My PID is %d\n", getpid());
    } else {
        // Parent process
        printf("Ana l-parent process! My child's PID is %d\n %d\n", pid, getppid());
    }
    // Both might print this
    printf("Process %d kaykml lkhdma.\n", getpid());
    return 0;
}