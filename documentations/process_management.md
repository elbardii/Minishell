
## 1. Definitions:

#### `pid_t fork(void);`
katkhla9 lina process jdid (`child process`) tib9 l2asl l'main process (`parent process`) kay3tih copy dyal memory, file descriptor... bwahd technique smitha `copy-On-Write`, katreturni l `child` `0` o l'parent katreturni lih `pid` dyal `child` w `-1` f 7alat tra chi mochkil, `child` kaykml mn lpoint flcode li wsal lih l`parent` fach 3yt l `fork`, btw rah makinach chi taratobiya binathom y3ni momkin `child` ykhdm lwl momkin `parent` ykhdm lwl, f7alat ma drty 3awtani `fork` mora lwla ght3awd tcree lina `child process` mn l`parent` wlkin maghtcreeyich wahd l`child` li drna flwl.

```c
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
        printf("Ana l-parent process! My child's PID is %d\n", pid);
    }
    // Both might print this
    printf("Process %d kaykml lkhdma.\n", getpid());
    return 0;
}
```

#### `pid_t wait(int *wstatus);`
ktkhali l'parent ytsna chi `child process` ymot, ktrj3 `PID` dyalo, katkhdm bach t7yd `zombie processes`, ila makin hta `child process` ktrj3 `-1` w kat7t `errno`. katcontroli l'order dyal l'execution.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // Child
        printf("Child (PID %d): Kansali daba.\n", getpid());
        exit(3); // Exit with status 3
    } else {
        // Parent
        printf("Parent (PID %d): Kantsna ay child...\n", getpid());
        int status;
        pid_t child_pid = wait(&status); // Wait for any child
        if (child_pid > 0) {
            if (WIFEXITED(status)) {
                printf("Parent: Child %d sala b status %d.\n", child_pid, WEXITSTATUS(status));
            }
        } else {
            perror("wait failed");
        }
    }
    return 0;
}
```

#### `pid_t waitpid(pid_t pid, int *wstatus, int options);`
hadi bhal `wait` ghahowa blast mntsnaw ay `child process` ysali kntsnaw process bdabt ysali.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
    pid_t target_child_pid = fork();

    if (target_child_pid < 0) {
        perror("fork failed");
        return 1;
    } else if (target_child_pid == 0) {
        // Child
        printf("Child (PID %d): Ghansali daba.\n", getpid());
        exit(4); // Exit with status 4
    } else {
        // Parent
        printf("Parent (PID %d): Kantsna ghir child %d...\n", getpid(), target_child_pid);
        int status;
        pid_t result_pid = waitpid(target_child_pid, &status, 0); // Wait for specific child
        if (result_pid == target_child_pid) {
             if (WIFEXITED(status)) {
                printf("Parent: Child %d sala b status %d.\n", result_pid, WEXITSTATUS(status));
             }
        } else {
            perror("waitpid failed");
        }
    }
    return 0;
}
```

#### `pid_t wait4(pid_t pid, int *wstatus, int options, struct rusage *rusage);` (w `wait3` b7alo tqriban f l function bla `pid`)
bhal `wait` w `waitpid` gha homa ky3tiwk hta `resource usage` f `struct rusage` gha howa `wait3` ktsna ay `child process` w `wait4` t9dr t3tiha `pid` dyalo.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // Child
        printf("Child (PID %d): Ghansali.\n", getpid());
        // Simulate some work
         for(long i=0; i< 10000000; ++i);
        exit(0);
    } else {
        // Parent
        printf("Parent (PID %d): Kantsna child %d b wait4...\n", getpid(), pid);
        int status;
        struct rusage usage;
        pid_t result_pid = wait4(pid, &status, 0, &usage); // Wait for specific child + usage

        if (result_pid == pid) {
             if (WIFEXITED(status)) {
                printf("Parent: Child %d sala.\n", result_pid);
                printf("  User CPU time: %ld.%06lds\n", usage.ru_utime.tv_sec, (long)usage.ru_utime.tv_usec);
             }
        } else {
            perror("wait4 failed");
        }
    }
    return 0;
}
```

## 2. Understanding:

ila bghina ncreeyiw wahd `child process` kn3yto 3la `fork()` li ktkhli lprocess ybda lkhdma mn wra lcall dyalha ktreturni lih `0` w ktreturni l `parent` `pid` dyal `child`, ila bghina ntsnawh n9dro n3yto 3la ay `wait` mn li chr7t f wahd `if statement` ila kan dak return mn ghir zero ydir `wait` binma sala lprocess dyal `child`.

## 3. `execve()` :

**Prototype:**
```c
#include <unistd.h>
int execve(const char *pathname, char *const argv[], char *const envp[]);
```

hiya `system call` f Linux/Unix li katkhali l'process li khddam daba ybdl rasso b programme jdid. Katms7 koulchi li kayn f la mémoire dyal lprocess l9dim (mn code, data, stack...), w katsayb plasso l-programme jdida li kat3tiha (`pathname`), m3a l-arguments (`argv`) w les variables dyal l'environment (`envp`). Mlli `execve()` katnaja7, l'process l9dim kaytna9a w l'programme jdid kaykhdem b nafs l'`PID`. `execve()` makatrd lik 7ta haja ila khdmat w `errno` ila tra chi mochkil, li kay3ni bli l'execution katsift l'massar jdid bla ma katsali l'function. Lmzyan f `execve()` howa f `fork()` w `execve()` m3a ba3dhiyathom: katdir `fork()` bach tkhl9 process jdida, w dak process (weld) kaydir `execve()` bach ybddl rasso b programme jdid, bla may2tr 3la lprocess l2asli (parent)

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Child (PID %d): Ghadi ndir execve l /bin/ls -l /tmp\n", getpid());

        char *args[] = {"/bin/ls", "-l", "/tmp", NULL}; // argv
        char *env[] = {NULL}; // Minimal environment

        execve("/bin/ls", args, env);

        // execve KAYKHESS MATRJE3CH ILA NEJ7AT!
        perror("execve failed"); // Ghadi t afficha ghir ila execve fashlat
        exit(1);

    } else {
        // Parent process
        printf("Parent (PID %d): Kantsna child %d...\n", getpid(), pid);
        wait(NULL); // Wait for child to finish
        printf("Parent: Child sala.\n");
    }
    return 0;
}
```
```