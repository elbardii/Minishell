#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

// Execute a command with output redirection
void execute_with_redirection(const char *command, const char *outfile) {
    pid_t pid = fork();  // System Call to create a new process
    
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid == 0) {  // Child process
        // Open a file for writing - System Call
        int fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        
        // Redirect stdout to the file - System Call
        if (dup2(fd, STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        
        // Close the file descriptor - System Call
        close(fd);
        
        // Split the command into args
        char *args[20];
        char *token;
        char cmd_copy[100];
        strcpy(cmd_copy, command);
        
        int i = 0;
        token = strtok(cmd_copy, " ");
        while (token != NULL && i < 19) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;
        
        // Execute the command - System Call
        execvp(args[0], args);
        
        // If we reach here, execvp failed
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {  // Parent process
        // Wait for child process to terminate - System Call
        waitpid(pid, NULL, 0);
    }
}

int main() {
    char outfile[50] = "output.txt";
    char command[100] = "ls -l";
    
    printf("Executing: %s > %s\n", command, outfile);
    execute_with_redirection(command, outfile);
    
    printf("Output saved to %s\n", outfile);
    
    return 0;
}