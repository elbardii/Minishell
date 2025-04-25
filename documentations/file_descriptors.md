# File Descriptors in Unix Systems

<div align="center">
  <a href="#understanding-system-calls"><img src="https://img.shields.io/badge/Read%20in-English 🇬🇧-blue?style=for-the-badge" alt="English"></a>
  <a href="#شنو-هي-الـ-system-calls"><img src="https://img.shields.io/badge/Read%20in-Darija 🇲🇦-green?style=for-the-badge" alt="Moroccan Darija"></a>
</div>

---

## Understanding System Calls

**System Calls** are the interface that allows your programs to request services from the operating system. In simple terms, they are "calls" sent to the system to perform tasks that your program lacks the permissions to do directly or that require hardware control.

### Importance of System Calls:

1. **Security**: They prevent programs from directly accessing hardware.
2. **Abstraction**: They allow programming without needing to know the intricate details of the hardware.
3. **Resource Management**: They enable the system to control available resources and how they're used.

## Main Types of System Calls:

### 1. Process Control:

- **`fork()`**: Creates a new process.
- **`exec()`**: Loads and executes a new program.
- **`wait()`, `waitpid()`**: Waits for a process to terminate.
- **`exit()`**: Terminates the current process.
- **`kill()`**: Sends a signal to a process.

```c
pid_t pid = fork();
if (pid == 0) {
    // Child process
    execl("/bin/ls", "ls", "-l", NULL);
    // If we reach here, execl failed
    exit(1);
} else if (pid > 0) {
    // Parent process
    wait(NULL);
}
```

### 2. File Management:

- **`open()`, `close()`**: Opens and closes files.
- **`read()`, `write()`**: Reads and writes data.
- **`lseek()`**: Changes the file position pointer.
- **`stat()`**: Gets file information.
- **`mkdir()`, `rmdir()`**: Creates and removes directories.
- **`dup()`, `dup2()`**: Duplicates file descriptors.

```c
int fd = open("file.txt", O_WRONLY | O_CREAT, 0644);
if (fd != -1) {
    write(fd, "Hello", 5);
    close(fd);
}
```

### 3. Device Management:

- **`ioctl()`**: Controls devices.
- **`mmap()`, `munmap()`**: Maps memory.

```c
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    printf("Terminal size: %d rows x %d columns\n", size.ws_row, size.ws_col);
    return 0;
}
```

### 4. Information Maintenance:

- **`getpid()`, `getppid()`**: Gets process IDs.
- **`getuid()`, `getgid()`**: Gets user or group IDs.
- **`uname()`**: Gets system information.
- **`time()`**: Gets the current time.

```c
pid_t pid = getpid();
printf("PID: %d\n", pid);
```

### 5. Communications:

- **`pipe()`**: Creates a communication channel.
- **`socket()`, `connect()`, `bind()`**: Network interfaces.
- **`shmget()`, `shmat()`**: Shared memory.
- **`msgget()`, `msgsnd()`, `msgrcv()`**: Message queues.
- **`semget()`, `semop()`**: Semaphores.

```c
int pipefd[2];
pipe(pipefd);
// pipefd[0] for reading, pipefd[1] for writing
```

### 6. Protection:

- **`chmod()`, `chown()`**: Changes file permissions or ownership.
- **`setuid()`, `setgid()`**: Changes user or group identity.

```c
chmod("file.txt", 0644);  // rw-r--r--
```

## How System Calls Work:

### 1. Direct Method:

```c
#include <unistd.h>
// ...
read(fd, buffer, size);
```

### 2. Through wrapper functions:

```c
#include <stdio.h>
// ...
FILE *f = fopen("file.txt", "r");
fread(buffer, 1, size, f);
fclose(f);
```

## System Call Mechanism:

1. **Program calls a System Call** (e.g., `read()`)
2. **CPU switches from User Mode to Kernel Mode**
3. **Kernel executes the requested function**
4. **Result returns to the program**
5. **CPU returns to User Mode**

## Important Concepts in System Calls:

### 1. User Mode vs Kernel Mode:

- **User Mode**: Normal processes run here. They don't have direct access to hardware.
- **Kernel Mode**: The system runs here. It has full access to hardware.

### 2. Blocking vs Non-Blocking Calls:

- **Blocking**: The process stops until the System Call completes (e.g., `read()` on an empty pipe).
- **Non-Blocking**: The process continues even if the System Call hasn't completed yet.

### 3. Interrupts:

- System Calls often use interrupts to notify the Kernel that they need service.

## Relevant System Calls for Minishell:

### 1. For Command Execution:
- **`fork()`**: Creates a new process for each command.
- **`execve()`**: Executes the new command.
- **`wait()`, `waitpid()`**: Waits for the command to finish.

### 2. For Redirection:
- **`open()`**: Opens files for input/output.
- **`dup2()`**: Redirects stdin/stdout to files.
- **`close()`**: Closes file descriptors.

### 3. For Pipes:
- **`pipe()`**: Creates a connection between two commands.

### 4. For Signal Handling:
- **`signal()`, `sigaction()`**: Sets signal handlers.

### 5. For Directory Operations:
- **`getcwd()`**: Gets the current directory (for the `pwd` command).
- **`chdir()`**: Changes the current directory (for the `cd` command).
- **`opendir()`, `readdir()`, `closedir()`**: Reads directory contents.

## Tips for Optimal System Call Usage:

1. **Always check return values** to detect errors.
2. **Use `perror()` or `strerror()`** to display error messages.
3. **Close resources** (like file descriptors) when you're done with them.
4. **Handle signals properly**, especially in interactive programs.
5. **Avoid race conditions** when using shared resources between multiple processes.

## Comprehensive Practical Example (Mini Minishell):

```c
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
```

---

<div dir="rtl">

## شنو هي الـ System Calls؟

الـ **System Calls** هي واجهة (interface) اللي كتسمح للبرامج ديالك باش تطلب خدمات من نظام التشغيل. بكلمات بسيطة، هي "نداءات" كندوزوها للنظام باش يدير لينا مهام ما عندناش الصلاحيات باش نديروها مباشرة، أو اللي محتاجة تحكم في hardware.

### الأهمية ديال System Calls:

1. **الأمان**: كتمنع البرامج من الوصول المباشر للـ hardware.
2. **التجريد (Abstraction)**: كتخلينا نبرمجو بدون ما نحتاجو نعرفو التفاصيل الدقيقة ديال الـ hardware.
3. **إدارة الموارد**: كتسمح للنظام باش يتحكم في الموارد المتاحة وكيفية استعمالها.

## أنواع الـ System Calls الرئيسية:

### 1. إدارة العمليات (Process Control):

- **`fork()`**: إنشاء عملية جديدة.
- **`exec()`**: تحميل وتنفيذ برنامج جديد.
- **`wait()`, `waitpid()`**: انتظار انتهاء عملية.
- **`exit()`**: إنهاء العملية الحالية.
- **`kill()`**: إرسال إشارة لعملية.

</div>

```c
pid_t pid = fork();
if (pid == 0) {
    // العملية الوليدة (child)
    execl("/bin/ls", "ls", "-l", NULL);
    // إذا وصلنا هنا فمعناه أن execl فشلت
    exit(1);
} else if (pid > 0) {
    // العملية الأب (parent)
    wait(NULL);
}
```

<div dir="rtl">

### 2. إدارة الملفات (File Management):

- **`open()`, `close()`**: فتح وإغلاق الملفات.
- **`read()`, `write()`**: قراءة وكتابة البيانات.
- **`lseek()`**: تغيير مؤشر موضع الملف.
- **`stat()`**: الحصول على معلومات عن الملف.
- **`mkdir()`, `rmdir()`**: إنشاء وحذف المجلدات.
- **`dup()`, `dup2()`**: تكرار الـ file descriptors.

</div>

```c
int fd = open("file.txt", O_WRONLY | O_CREAT, 0644);
if (fd != -1) {
    write(fd, "Hello", 5);
    close(fd);
}
```

<div dir="rtl">

### 3. إدارة الجهاز (Device Management):

- **`ioctl()`**: التحكم في الأجهزة.
- **`mmap()`, `munmap()`**: خرائط الذاكرة.

</div>

```c
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    printf("Terminal size: %d rows x %d columns\n", size.ws_row, size.ws_col);
    return 0;
}
```

<div dir="rtl">

### 4. إدارة المعلومات (Information Maintenance):

- **`getpid()`, `getppid()`**: الحصول على معرف العملية.
- **`getuid()`, `getgid()`**: الحصول على معرف المستخدم أو المجموعة.
- **`uname()`**: الحصول على معلومات النظام.
- **`time()`**: الحصول على الوقت الحالي.

</div>

```c
pid_t pid = getpid();
printf("PID: %d\n", pid);
```

<div dir="rtl">

### 5. الاتصالات (Communications):

- **`pipe()`**: إنشاء قناة اتصال.
- **`socket()`, `connect()`, `bind()`**: واجهات الشبكة.
- **`shmget()`, `shmat()`**: الذاكرة المشتركة.
- **`msgget()`, `msgsnd()`, `msgrcv()`**: قوائم الرسائل.
- **`semget()`, `semop()`**: المجففات (Semaphores).

</div>

```c
int pipefd[2];
pipe(pipefd);
// pipefd[0] للقراءة، pipefd[1] للكتابة
```

<div dir="rtl">

### 6. الحماية (Protection):

- **`chmod()`, `chown()`**: تغيير أذونات أو ملكية الملف.
- **`setuid()`, `setgid()`**: تغيير هوية المستخدم أو المجموعة.

</div>

```c
chmod("file.txt", 0644);  // rw-r--r--
```

<div dir="rtl">

## كيفاش كتخدم الـ System Calls:

### 1. الطريقة المباشرة:

</div>

```c
#include <unistd.h>
// ...
read(fd, buffer, size);
```

<div dir="rtl">

### 2. من خلال wrapper functions:

</div>

```c
#include <stdio.h>
// ...
FILE *f = fopen("file.txt", "r");
fread(buffer, 1, size, f);
fclose(f);
```

<div dir="rtl">

## آلية عمل الـ System Calls:

1. **البرنامج كيستدعي System Call** (مثلا `read()`)
2. **CPU كيتبدل من User Mode إلى Kernel Mode**
3. **الـ Kernel كينفذ الوظيفة المطلوبة**
4. **النتيجة كترجع للبرنامج**
5. **CPU كيرجع لـ User Mode**

## المفاهيم المهمة في System Calls:

### 1. User Mode vs Kernel Mode:

- **User Mode**: العمليات العادية كتشتغل فيه. ماعندهاش وصول مباشر للـ hardware.
- **Kernel Mode**: النظام كيخدم فيه. عندو وصول كامل للـ hardware.

### 2. Blocking vs Non-Blocking Calls:

- **Blocking**: العملية كتوقف حتى تكمل الـ System Call (مثلا `read()` على pipe فارغة).
- **Non-Blocking**: العملية كتستمر حتى وإلا الـ System Call مازال ماكملاتش.

### 3. المقاطعة (Interrupts):

- System Calls غالبا كتستعمل المقاطعات باش تخبر الـ Kernel أنها محتاجة خدمة.

## الـ System Calls المهمة فمشروع Minishell:

### 1. للـ Command Execution:
- **`fork()`**: إنشاء عملية جديدة لكل أمر.
- **`execve()`**: تنفيذ الأمر الجديد.
- **`wait()`, `waitpid()`**: انتظار انتهاء الأمر.

### 2. للـ Redirection:
- **`open()`**: فتح الملفات للـ input/output.
- **`dup2()`**: توجيه stdin/stdout للملفات.
- **`close()`**: إغلاق الـ file descriptors.

### 3. للـ Pipes:
- **`pipe()`**: إنشاء اتصال بين أمرين.

### 4. للـ Signal Handling:
- **`signal()`, `sigaction()`**: تعيين handlers للإشارات.

### 5. للـ Directory Operations:
- **`getcwd()`**: الحصول على المجلد الحالي (خاصة لـ `pwd` command).
- **`chdir()`**: تغيير المجلد الحالي (خاصة لـ `cd` command).
- **`opendir()`, `readdir()`, `closedir()`**: قراءة محتويات المجلد.

## نصائح للاستخدام الأمثل للـ System Calls:

1. **دائما تحقق من قيم العودة** للكشف عن الأخطاء.
2. **استخدم `perror()` أو `strerror()`** لعرض رسائل الخطأ.
3. **أغلق الموارد** (مثل الـ file descriptors) عندما تنتهي من استخدامها.
4. **تعامل مع signals** بشكل صحيح، خاصة في البرامج التفاعلية.
5. **تجنب race conditions** عند استخدام موارد مشتركة بين عمليات متعددة.

## مثال عملي شامل (مصغر من Minishell):

</div>

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

// تنفيذ أمر مع إعادة توجيه المخرجات
void execute_with_redirection(const char *command, const char *outfile) {
    pid_t pid = fork();  // System Call لإنشاء عملية جديدة
    
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid == 0) {  // العملية الوليدة
        // فتح ملف للكتابة - System Call
        int fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        
        // توجيه stdout إلى الملف - System Call
        if (dup2(fd, STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        
        // إغلاق الـ file descriptor - System Call
        close(fd);
        
        // تجزئة الأمر إلى args
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
        
        // تنفيذ الأمر - System Call
        execvp(args[0], args);
        
        // إذا وصلنا هنا فمعناه أن execvp فشلت
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {  // العملية الأب
        // انتظار انتهاء العملية الوليدة - System Call
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
```

<div dir="rtl">

## خلاصة:

الـ System Calls هي المفتاح لفهم كيفية تفاعل البرامج مع نظام التشغيل. فمشروع Minishell، غادي تستعمل العديد من الـ System Calls باش تنفذ وظائف مختلفة بحال تنفيذ الأوامر، توجيه المدخلات/المخرجات، إنشاء الـ pipes، والتعامل مع الإشارات.

فهم عميق للـ System Calls غادي يساعدك تبني shell متكامل وفعال!

</div>

# Process Management in Unix Systems

<div align="center">
  <a href="#process-management-fundamentals"><img src="https://img.shields.io/badge/Read%20in-English 🇬🇧-blue?style=for-the-badge" alt="English"></a>
  <a href="#مفاهيم-إدارة-العمليات"><img src="https://img.shields.io/badge/Read%20in-Darija 🇲🇦-green?style=for-the-badge" alt="Moroccan Darija"></a>
</div>

---

## Process Management Fundamentals

**Process Management** is a crucial aspect of Unix-based operating systems. A process is simply a program in execution, which has its own memory space, resources, and state. Understanding how processes work is essential for developing a shell like Minishell.

### Key Concepts in Process Management:

1. **Process Creation and Execution**: Using `fork()` and the `exec` family of functions
2. **Process Waiting**: Using `wait()` and `waitpid()` functions
3. **Process Termination**: Via `exit()` function and signals
4. **Signal Handling**: Dealing with keyboard interrupts and other signals
5. **Process Groups and Sessions**: How processes are organized in the system

## 1. Process Creation and Control

### 1.1. Creating Processes (`fork()`)

```c
#include <unistd.h>
pid_t fork(void);
```

The `fork()` system call creates a copy (clone) of the current process, with the following return values:
- In the parent process: PID of the newly created child process
- In the child process: 0
- On error: -1

#### Example:
```c
pid_t pid = fork();
if (pid < 0) {
    // Error handling
    perror("fork failed");
} else if (pid == 0) {
    // Child process
    printf("I am the child process PID=%d, Parent PID=%d\n", getpid(), getppid());
} else {
    // Parent process
    printf("I am the parent process PID=%d, Child PID=%d\n", getpid(), pid);
}
```

### 1.2. Executing Programs (`exec` family)

#### (a) `execve()`: The base function
```c
#include <unistd.h>
int execve(const char *pathname, char *const argv[], char *const envp[]);
```

#### (b) Easier-to-use wrappers:
- `execl()`: List of arguments
- `execlp()`: Search in PATH
- `execle()`: With environment variables
- `execv()`: Array of arguments
- `execvp()`: Search in PATH with array of arguments
- `execvpe()`: With environment variables and array of arguments

**Important note**: The `exec` functions replace the current process with the new program. Code after an `exec` call will not execute unless the call fails.

#### Example:
```c
if (fork() == 0) {
    // Child process
    char *args[] = {"ls", "-l", NULL};
    execvp("ls", args);
    // This code only executes if execvp fails
    perror("execvp failed");
    exit(1);
}
```

### 1.3. Waiting for Processes (`wait()`, `waitpid()`)

```c
#include <sys/wait.h>
pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int options);
```

- `wait()`: Waits for any child process to terminate
- `waitpid()`: Waits for a specific child process
  - `pid > 0`: Wait for the process with this PID
  - `pid = 0`: Wait for any process in the same group
  - `pid = -1`: Wait for any process (like `wait()`)
  - `pid < -1`: Wait for any process in a specific group

#### Important options for `waitpid()`:
- `WNOHANG`: Don't block the parent if the child is still running
- `WUNTRACED`: Get information about stopped processes (e.g., by SIGSTOP)
- `WCONTINUED`: Get information about continued processes (e.g., by SIGCONT)

#### Analyzing exit status:
- `WIFEXITED(status)`: Whether the process exited normally
- `WEXITSTATUS(status)`: The exit value
- `WIFSIGNALED(status)`: Whether the process was terminated by a signal
- `WTERMSIG(status)`: The number of the signal that caused termination
- `WIFSTOPPED(status)`: Whether the process was stopped (with `WUNTRACED`)
- `WSTOPSIG(status)`: The number of the signal that caused stopping

#### Example:
```c
pid_t pid = fork();
if (pid == 0) {
    // Child process
    sleep(2);
    exit(42);  // Exit with status 42
} else if (pid > 0) {
    // Parent process
    int status;
    pid_t waited_pid = waitpid(pid, &status, 0);
    
    if (waited_pid == -1) {
        perror("waitpid failed");
    } else if (WIFEXITED(status)) {
        printf("Child exited with status: %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("Child killed by signal: %d\n", WTERMSIG(status));
    }
}
```

### 1.4. Terminating Processes (`exit()`)

```c
#include <stdlib.h>
void exit(int status);

#include <unistd.h>
void _exit(int status);
```

- `exit()`: Execute functions registered with `atexit()` and close streams
- `_exit()`: Immediately terminate the process (low-level)

#### Example:
```c
if (some_error) {
    fprintf(stderr, "Error: %s\n", strerror(errno));
    exit(EXIT_FAILURE);  // Defined in stdlib.h as 1
}

// Success
exit(EXIT_SUCCESS);  // Defined in stdlib.h as 0
```

### 1.5. Sending Signals (`kill()`)

```c
#include <signal.h>
int kill(pid_t pid, int sig);
```

- `pid > 0`: Send the signal to the specified process
- `pid = 0`: Send the signal to all processes in the same group
- `pid = -1`: Send the signal to all processes that the current process has permission to send signals to
- `pid < -1`: Send the signal to all processes in the specified group

#### Important signals:
- `SIGTERM` (15): Request termination (default signal for `kill`)
- `SIGKILL` (9): Force process termination (cannot be handled)
- `SIGSTOP` (19): Stop the process (cannot be handled)
- `SIGCONT` (18): Continue the process after stopping
- `SIGINT` (2): Interrupt from keyboard (typically Ctrl+C)
- `SIGQUIT` (3): Quit from keyboard (typically Ctrl+\\)
- `SIGHUP` (1): Hangup - often used to reload configuration

#### Example:
```c
pid_t pid = /* PID of the process */;

// Send SIGTERM
if (kill(pid, SIGTERM) == -1) {
    perror("kill failed");
}

// Wait a moment
sleep(2);

// Check if the process is still running, if so, use SIGKILL
if (kill(pid, 0) == 0) {
    printf("Process still running, sending SIGKILL\n");
    kill(pid, SIGKILL);
}
```

## 2. Applications in Minishell

### 2.1. Executing a Simple Command

```c
void execute_command(char *command) {
    // Split the command into arguments
    char *args[MAX_ARGS];
    int arg_count = 0;
    char *token = strtok(command, " \t\n");
    
    while (token != NULL && arg_count < MAX_ARGS - 1) {
        args[arg_count++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[arg_count] = NULL;
    
    if (arg_count == 0) return;
    
    // Execute builtin commands
    if (strcmp(args[0], "cd") == 0) {
        // Execute cd
        if (args[1] == NULL) {
            chdir(getenv("HOME"));
        } else {
            if (chdir(args[1]) != 0) {
                perror("cd");
            }
        }
        return;
    } else if (strcmp(args[0], "exit") == 0) {
        // Execute exit
        exit(args[1] ? atoi(args[1]) : 0);
    }
    
    // Create a new process to execute the command
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork failed");
    } else if (pid == 0) {
        // Child process
        execvp(args[0], args);
        // This code executes only if execvp fails
        fprintf(stderr, "%s: command not found\n", args[0]);
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        
        // Update $? (especially in the Minishell project)
        if (WIFEXITED(status)) {
            last_exit_status = WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
            last_exit_status = 128 + WTERMSIG(status);
        }
    }
}
```

### 2.2. Executing Commands with Pipes

```c
void execute_pipeline(char *commands[], int count) {
    int pipes[MAX_COMMANDS-1][2];
    pid_t pids[MAX_COMMANDS];
    
    // Create pipes
    for (int i = 0; i < count-1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            return;
        }
    }
    
    // Create processes for each command
    for (int i = 0; i < count; i++) {
        pids[i] = fork();
        
        if (pids[i] < 0) {
            perror("fork");
            // Close pipes and clean up
            for (int j = 0; j < count-1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            return;
        }
        
        if (pids[i] == 0) {
            // Child process
            
            // Connect input from previous pipe (if not first command)
            if (i > 0) {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }
            
            // Connect output to next pipe (if not last command)
            if (i < count-1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            
            // Close all pipe descriptors
            for (int j = 0; j < count-1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            
            // Split command and execute
            char *args[MAX_ARGS];
            int arg_count = 0;
            char cmd_copy[BUFFER_SIZE];
            strcpy(cmd_copy, commands[i]);
            
            char *token = strtok(cmd_copy, " \t\n");
            while (token != NULL && arg_count < MAX_ARGS - 1) {
                args[arg_count++] = token;
                token = strtok(NULL, " \t\n");
            }
            args[arg_count] = NULL;
            
            execvp(args[0], args);
            fprintf(stderr, "%s: command not found\n", args[0]);
            exit(EXIT_FAILURE);
        }
    }
    
    // Parent process
    
    // Close all pipe descriptors in the parent
    for (int i = 0; i < count-1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    
    // Wait for all child processes to finish
    int last_status = 0;
    for (int i = 0; i < count; i++) {
        int status;
        waitpid(pids[i], &status, 0);
        
        // Store exit status of only the last command
        if (i == count-1) {
            if (WIFEXITED(status)) {
                last_exit_status = WEXITSTATUS(status);
            } else if (WIFSIGNALED(status)) {
                last_exit_status = 128 + WTERMSIG(status);
            }
        }
    }
}
```

### 2.3. Handling Signals

```c
#include <signal.h>

// Pointer to the current foreground process (if any)
pid_t foreground_pid = 0;

// Signal handler
void signal_handler(int signum) {
    if (signum == SIGINT) {
        // Ctrl+C
        if (foreground_pid > 0) {
            // Send the signal to the foreground process
            kill(foreground_pid, SIGINT);
        } else {
            // If there's no foreground process, display a new prompt
            write(STDOUT_FILENO, "\n$ ", 3);
        }
    } else if (signum == SIGQUIT) {
        // Ctrl+\
        // In Minishell, we ignore this signal
    }
}

// Set up signal handling
void setup_signals() {
    struct sigaction sa;
    
    // Set up the signal handler
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    // Register the handler for SIGINT (Ctrl+C)
    sigaction(SIGINT, &sa, NULL);
    
    // Register the handler for SIGQUIT (Ctrl+\)
    sigaction(SIGQUIT, &sa, NULL);
    
    // Ignore SIGTSTP (Ctrl+Z) in the shell itself
    signal(SIGTSTP, SIG_IGN);
}

// Set the foreground process
void set_foreground_process(pid_t pid) {
    foreground_pid = pid;
}

// Execute a command with signal handling
void execute_with_signals(char *args[]) {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
    } else if (pid == 0) {
        // Child process
        
        // Reset default signal handlers
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        
        execvp(args[0], args);
        fprintf(stderr, "%s: command not found\n", args[0]);
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        
        // Set this process as the foreground process
        set_foreground_process(pid);
        
        // Wait for the process to finish
        int status;
        waitpid(pid, &status, 0);
        
        // Reset foreground_pid
        set_foreground_process(0);
        
        // Update exit status
        if (WIFEXITED(status)) {
            last_exit_status = WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
            // If the process was terminated by a signal (e.g., Ctrl+C)
            last_exit_status = 128 + WTERMSIG(status);
            
            // Display appropriate message when process is killed by a signal
            if (WTERMSIG(status) == SIGINT) {
                printf("\n");
            } else if (WTERMSIG(status) == SIGQUIT) {
                printf("Quit (core dumped)\n");
            }
        }
    }
}
```

## 3. Advanced Concepts in Process Management

### 3.1. Process Groups and Sessions

- **Process Group**: A collection of related processes that can be managed together
- **Session**: A collection of process groups, typically associated with a terminal session

```c
#include <unistd.h>

// Get or set process group ID
pid_t getpgrp(void);
pid_t getpgid(pid_t pid);
int setpgid(pid_t pid, pid_t pgid);

// Get or set session ID
pid_t getsid(pid_t pid);
pid_t setsid(void);
```

### 3.2. Process Priority

```c
#include <sys/resource.h>

// Set/get priority
int getpriority(int which, id_t who);
int setpriority(int which, id_t who, int value);
```

### 3.3. Daemon Processes

```c
// Create a daemon
pid_t pid = fork();
if (pid > 0) exit(0);  // Exit parent
setsid();              // Create new session
// Redirect I/O, change directory, etc.
```

## 4. Tips and Guidelines for Minishell Project

1. **Start with simple command execution**:
   - Begin by executing commands without pipes/redirections.
   - Learn how to parse commands and convert them to argv array.

2. **Handle shell built-ins**:
   - Built-in commands like cd, echo, pwd should be executed directly (without fork/exec).
   - Make sure exit properly exits the shell.

3. **Handle the PATH search**:
   - Use getenv("PATH") to get the search path.
   - Test each path to see if the file exists and is executable.

4. **Handle signals correctly**:
   - Set up signal handlers before entering the main loop.
   - Make sure to handle Ctrl+C, Ctrl+D, Ctrl+\ properly.

5. **Check for memory leaks**:
   - Processes terminate, but leaks in the parent process (the shell) accumulate.
   - Use valgrind to detect memory leaks.

6. **Handle exit status**:
   - Save the exit status of the last executed command for use in $?.
   - Remember that signals have special exit statuses (128 + signal_number).

7. **Test edge cases**:
   - Non-existent commands.
   - Invalid filenames.
   - Redirections to/from inaccessible files.
   - Handling of single and double quotes.

## 5. Comprehensive Example

### 5.1. Simple Shell Main Loop

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

#define BUFFER_SIZE 1024
#define MAX_ARGS 64

// Global variables
int last_exit_status = 0;
char *current_line = NULL;

// Display prompt
void display_prompt() {
    char cwd[BUFFER_SIZE];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s$ ", cwd);
    } else {
        printf("$ ");
    }
    fflush(stdout);
}

// Execute built-in command
int execute_builtin(char *args[]) {
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            // cd without arguments goes to HOME
            char *home = getenv("HOME");
            if (home && chdir(home) != 0) {
                perror("cd");
            }
        } else if (chdir(args[1]) != 0) {
            perror("cd");
        }
        return 1;
    } else if (strcmp(args[0], "exit") == 0) {
        // Execute exit
        int status = 0;
        if (args[1] != NULL) {
            status = atoi(args[1]);
        }
        exit(status);
    } else if (strcmp(args[0], "pwd") == 0) {
        // Execute pwd
        char cwd[BUFFER_SIZE];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("pwd");
        }
        return 1;
    }
    
    // Not a built-in command
    return 0;
}

// Signal handler
void signal_handler(int signum) {
    if (signum == SIGINT) {
        // Ctrl+C
        printf("\n");
        if (current_line) {
            free(current_line);
            current_line = NULL;
        }
        display_prompt();
    }
}

// Set up signal handling
void setup_signals() {
    struct sigaction sa;
    
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    sigaction(SIGINT, &sa, NULL);
    signal(SIGQUIT, SIG_IGN);
}

// Execute external command
void execute_external(char *args[]) {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
    } else if (pid == 0) {
        // Child process
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        
        execvp(args[0], args);
        fprintf(stderr, "%s: command not found\n", args[0]);
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            last_exit_status = WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
            last_exit_status = 128 + WTERMSIG(status);
            if (WTERMSIG(status) == SIGINT) {
                printf("\n");
            }
        }
    }
}

int main() {
    char *input;
    size_t input_size = 0;
    
    // Set up signal handling
    setup_signals();
    
    while (1) {
        display_prompt();
        
        // Read input
        ssize_t read = getline(&input, &input_size, stdin);
        current_line = input;
        
        if (read == -1) {
            // EOF (Ctrl+D)
            printf("exit\n");
            break;
        }
        
        // Remove newline
        if (read > 0 && input[read-1] == '\n') {
            input[read-1] = '\0';
        }
        
        // Ignore empty lines
        if (strlen(input) == 0) {
            continue;
        }
        
        // Split input into arguments
        char *args[MAX_ARGS];
        int arg_count = 0;
        
        char *token = strtok(input, " \t");
        while (token != NULL && arg_count < MAX_ARGS - 1) {
            args[arg_count++] = token;
            token = strtok(NULL, " \t");
        }
        args[arg_count] = NULL;
        
        if (arg_count > 0) {
            // Try to execute as a built-in command
            if (!execute_builtin(args)) {
                // If not a built-in, execute as an external command
                execute_external(args);
            }
        }
        
        current_line = NULL;
    }
    
    free(input);
    return last_exit_status;
}
```

## Conclusion

A deep understanding of process management is essential for building a robust and efficient shell. The key concepts are:

1. **Process creation** using `fork()`
2. **Program execution** using the `exec` family
3. **Waiting for processes** using `wait()` and `waitpid()`
4. **Signal handling** for Ctrl+C and other signals
5. **Exit status management** for executed commands

When building your Minishell, you'll combine these concepts with pipes, redirections, and parsing to implement a complete shell.

---

<div dir="rtl">

## مفاهيم إدارة العمليات

**إدارة العمليات** هي جانب مهم من جوانب أنظمة التشغيل المستندة إلى يونكس. العملية هي ببساطة برنامج قيد التنفيذ، ولديها مساحة ذاكرة خاصة بها، وموارد، وحالة. فهم كيفية عمل العمليات ضروري لتطوير shell مثل Minishell.

### المفاهيم الرئيسية في إدارة العمليات:

1. **إنشاء وتنفيذ العمليات**: باستخدام `fork()` وعائلة وظائف `exec`
2. **انتظار العمليات**: باستخدام وظائف `wait()` و`waitpid()`
3. **إنهاء العمليات**: عبر وظيفة `exit()` والإشارات
4. **معالجة الإشارات**: التعامل مع مقاطعات لوحة المفاتيح وإشارات أخرى
5. **مجموعات العمليات والجلسات**: كيفية تنظيم العمليات في النظام

## 1. إنشاء العمليات والتحكم فيها

### 1.1. إنشاء العمليات (`fork()`)

```c
#include <unistd.h>
pid_t fork(void);
```

استدعاء النظام `fork()` ينشئ نسخة (استنساخ) من العملية الحالية، مع قيم العودة التالية:
- في العملية الأب: PID للعملية الجديدة المنشأة
- في العملية الوليدة: 0
- في حالة الخطأ: -1

#### مثال:
```c
pid_t pid = fork();
if (pid < 0) {
    // معالجة الأخطاء
    perror("fork failed");
} else if (pid == 0) {
    // العملية الوليدة
    printf("أنا العملية الوليدة PID=%d, Parent PID=%d\n", getpid(), getppid());
} else {
    // العملية الأب
    printf("أنا العملية الأب PID=%d, Child PID=%d\n", getpid(), pid);
}
```

### 1.2. تنفيذ البرامج (عائلة `exec`)

#### (أ) `execve()`: الوظيفة الأساسية
```c
#include <unistd.h>
int execve(const char *pathname, char *const argv[], char *const envp[]);
```

#### (ب) واجهات أسهل للاستخدام:
- `execl()`: قائمة من الوسائط
- `execlp()`: البحث في PATH
- `execle()`: مع متغيرات البيئة
- `execv()`: مصفوفة من الوسائط
- `execvp()`: البحث في PATH مع مصفوفة من الوسائط
- `execvpe()`: مع متغيرات البيئة ومصفوفة من الوسائط

**ملاحظة مهمة**: وظائف `exec` تستبدل العملية الحالية بالبرنامج الجديد. الشفرة بعد استدعاء `exec` لن تنفذ إلا إذا فشل الاستدعاء.

#### مثال:
```c
if (fork() == 0) {
    // العملية الوليدة
    char *args[] = {"ls", "-l", NULL};
    execvp("ls", args);
    // هذه الشفرة تنفذ فقط إذا فشل execvp
    perror("execvp failed");
    exit(1);
}
```

### 1.3. انتظار العمليات (`wait()`, `waitpid()`)

```c
#include <sys/wait.h>
pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int options);
```

- `wait()`: ينتظر أي عملية وليدة لتنتهي
- `waitpid()`: ينتظر عملية وليدة محددة
  - `pid > 0`: انتظار العملية بهذا PID
  - `pid = 0`: انتظار أي عملية في نفس المجموعة
  - `pid = -1`: انتظار أي عملية (مثل `wait()`)
  - `pid < -1`: انتظار أي عملية في مجموعة محددة

#### خيارات مهمة لـ `waitpid()`:
- `WNOHANG`: لا تحجب العملية الأب إذا كانت العملية الوليدة لا تزال قيد التشغيل
- `WUNTRACED`: الحصول على معلومات حول العمليات المتوقفة (مثلاً، بواسطة SIGSTOP)
- `WCONTINUED`: الحصول على معلومات حول العمليات المستمرة (مثلاً، بواسطة SIGCONT)

#### تحليل حالة الخروج:
- `WIFEXITED(status)`: ما إذا كانت العملية قد خرجت بشكل طبيعي
- `WEXITSTATUS(status)`: قيمة الخروج
- `WIFSIGNALED(status)`: ما إذا كانت العملية قد أنهيت بواسطة إشارة
- `WTERMSIG(status)`: رقم الإشارة التي تسببت في الإنهاء
- `WIFSTOPPED(status)`: ما إذا كانت العملية قد توقفت (مع `WUNTRACED`)
- `WSTOPSIG(status)`: رقم الإشارة التي تسببت في التوقف

#### مثال:
```c
pid_t pid = fork();
if (pid == 0) {
    // العملية الوليدة
    sleep(2);
    exit(42);  // الخروج بحالة 42
} else if (pid > 0) {
    // العملية الأب
    int status;
    pid_t waited_pid = waitpid(pid, &status, 0);
    
    if (waited_pid == -1) {
        perror("waitpid failed");
    } else if (WIFEXITED(status)) {
        printf("Child exited with status: %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("Child killed by signal: %d\n", WTERMSIG(status));
    }
}
```

### 1.4. إنهاء العمليات (`exit()`)

```c
#include <stdlib.h>
void exit(int status);

#include <unistd.h>
void _exit(int status);
```

- `exit()`: تنفيذ الوظائف المسجلة مع `atexit()` وإغلاق الدفق (streams)
- `_exit()`: إنهاء العملية فوراً (مستوى منخفض)

#### مثال:
```c
if (some_error) {
    fprintf(stderr, "Error: %s\n", strerror(errno));
    exit(EXIT_FAILURE);  // معرف في stdlib.h كـ 1
}

// نجاح
exit(EXIT_SUCCESS);  // معرف في stdlib.h كـ 0
```

### 1.5. إرسال الإشارات (`kill()`)

```c
#include <signal.h>
int kill(pid_t pid, int sig);
```

- `pid > 0`: إرسال الإشارة إلى العملية المحددة
- `pid = 0`: إرسال الإشارة إلى جميع العمليات في نفس المجموعة
- `pid = -1`: إرسال الإشارة إلى جميع العمليات التي تملك العملية الحالية صلاحية إرسال إشارات إليها
- `pid < -1`: إرسال الإشارة إلى جميع العمليات في المجموعة المحددة

#### إشارات مهمة:
- `SIGTERM` (15): طلب إنهاء (الإشارة الافتراضية لـ `kill`)
- `SIGKILL` (9): إجبار إنهاء العملية (لا يمكن التعامل معها)
- `SIGSTOP` (19): إيقاف العملية (لا يمكن التعامل معها)
- `SIGCONT` (18): استمرار العملية بعد التوقف
- `SIGINT` (2): مقاطعة من لوحة المفاتيح (عادةً Ctrl+C)
- `SIGQUIT` (3): خروج من لوحة المفاتيح (عادةً Ctrl+\\)
- `SIGHUP` (1): قطع الاتصال - غالباً ما يستخدم لإعادة تحميل التكوين

#### مثال:
```c
pid_t pid = /* PID للعملية */;

// إرسال SIGTERM
if (kill(pid, SIGTERM) == -1) {
    perror("kill failed");
}

// انتظار لحظة
sleep(2);

// التحقق مما إذا كانت العملية لا تزال قيد التشغيل، إذا كانت كذلك، استخدم SIGKILL
if (kill(pid, 0) == 0) {
    printf("Process still running, sending SIGKILL\n");
    kill(pid, SIGKILL);
}
```

## 2. تطبيقات في Minishell

### 2.1. تنفيذ أمر بسيط

```c
void execute_command(char *command) {
    // تقسيم الأمر إلى وسائط
    char *args[MAX_ARGS];
    int arg_count = 0;
    char *token = strtok(command, " \t\n");
    
    while (token != NULL && arg_count < MAX_ARGS - 1) {
        args[arg_count++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[arg_count] = NULL;
    
    if (arg_count == 0) return;
    
    // تنفيذ أوامر مدمجة
    if (strcmp(args[0], "cd") == 0) {
        // تنفيذ cd
        if (args[1] == NULL) {
            chdir(getenv("HOME"));
        } else {
            if (chdir(args[1]) != 0) {
                perror("cd");
            }
        }
        return;
    } else if (strcmp(args[0], "exit") == 0) {
        // تنفيذ exit
        exit(args[1] ? atoi(args[1]) : 0);
    }
    
    // إنشاء عملية جديدة لتنفيذ الأمر
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork failed");
    } else if (pid == 0) {
        // العملية الوليدة
        execvp(args[0], args);
        // هذه الشفرة تنفذ فقط إذا فشل execvp
        fprintf(stderr, "%s: command not found\n", args[0]);
        exit(EXIT_FAILURE);
    } else {
        // العملية الأب
        int status;
        waitpid(pid, &status, 0);
        
        // تحديث $? (خصوصاً في مشروع Minishell)
        if (WIFEXITED(status)) {
            last_exit_status = WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
            last_exit_status = 128 + WTERMSIG(status);
        }
    }
}
```

### 2.2. تنفيذ أوامر مع pipes

```c
void execute_pipeline(char *commands[], int count) {
    int pipes[MAX_COMMANDS-1][2];
    pid_t pids[MAX_COMMANDS];
    
    // إنشاء pipes
    for (int i = 0; i < count-1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            return;
        }
    }
    
    // إنشاء عمليات لكل أمر
    for (int i = 0; i < count; i++) {
        pids[i] = fork();
        
        if (pids[i] < 0) {
            perror("fork");
            // إغلاق pipes والتنظيف
            for (int j = 0; j < count-1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            return;
        }
        
        if (pids[i] == 0) {
            // العملية الوليدة
            
            // توصيل المدخلات من pipe السابق (إذا لم يكن هذا هو الأمر الأول)
            if (i > 0) {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }
            
            // توصيل المخرجات إلى pipe التالي (إذا لم يكن هذا هو الأمر الأخير)
            if (i < count-1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            
            // إغلاق جميع pipe descriptors
            for (int j = 0; j < count-1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            
            // تقسيم الأمر وتنفيذه
            char *args[MAX_ARGS];
            int arg_count = 0;
            char cmd_copy[BUFFER_SIZE];
            strcpy(cmd_copy, commands[i]);
            
            char *token = strtok(cmd_copy, " \t\n");
            while (token != NULL && arg_count < MAX_ARGS - 1) {
                args[arg_count++] = token;
                token = strtok(NULL, " \t\n");
            }
            args[arg_count] = NULL;
            
            execvp(args[0], args);
            fprintf(stderr, "%s: command not found\n", args[0]);
            exit(EXIT_FAILURE);
        }
    }
    
    // العملية الأب
    
    // إغلاق جميع pipe descriptors في العملية الأب
    for (int i = 0; i < count-1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    
    // انتظار انتهاء جميع العمليات الوليدة
    int last_status = 0;
    for (int i = 0; i < count; i++) {
        int status;
        waitpid(pids[i], &status, 0);
        
        // تخزين حالة الخروج للأمر الأخير فقط
        if (i == count-1) {
            if (WIFEXITED(status)) {
                last_exit_status = WEXITSTATUS(status);
            } else if (WIFSIGNALED(status)) {
                last_exit_status = 128 + WTERMSIG(status);
            }
        }
    }
}
```

### 2.3. التعامل مع الإشارات

```c
#include <signal.h>

// مؤشر إلى العملية الأمامية الحالية (إذا وجدت)
pid_t foreground_pid = 0;

// معالج الإشارات
void signal_handler(int signum) {
    if (signum == SIGINT) {
        // Ctrl+C
        if (foreground_pid > 0) {
            // إرسال الإشارة إلى العملية الأمامية
            kill(foreground_pid, SIGINT);
        } else {
            // إذا لم تكن هناك عملية أمامية، عرض prompt جديد
            write(STDOUT_FILENO, "\n$ ", 3);
        }
    } else if (signum == SIGQUIT) {
        // Ctrl+\
        // في Minishell، نتجاهل هذه الإشارة
    }
}

// إعداد معالجة الإشارات
void setup_signals() {
    struct sigaction sa;
    
    // إعداد معالج الإشارات
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    // تسجيل المعالج لـ SIGINT (Ctrl+C)
    sigaction(SIGINT, &sa, NULL);
    
    // تسجيل المعالج لـ SIGQUIT (Ctrl+\)
    sigaction(SIGQUIT, &sa, NULL);
    
    // تجاهل SIGTSTP (Ctrl+Z) في الـ shell نفسه
    signal(SIGTSTP, SIG_IGN);
}

// تعيين العملية الأمامية
void set_foreground_process(pid_t pid) {
    foreground_pid = pid;
}

// تنفيذ أمر مع معالجة الإشارات
void execute_with_signals(char *args[]) {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
    } else if (pid == 0) {
        // العملية الوليدة
        
        // إعادة تعيين معالجات الإشارات الافتراضية
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        
        execvp(args[0], args);
        fprintf(stderr, "%s: command not found\n", args[0]);
        exit(EXIT_FAILURE);
    } else {
        // العملية الأب
        
        // تعيين هذه العملية كعملية أمامية
        set_foreground_process(pid);
        
        // انتظار انتهاء العملية
        int status;
        waitpid(pid, &status, 0);
        
        // إعادة تعيين foreground_pid
        set_foreground_process(0);
        
        // تحديث حالة الخروج
        if (WIFEXITED(status)) {
            last_exit_status = WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
            // إذا تم إنهاء العملية بواسطة إشارة (مثل Ctrl+C)
            last_exit_status = 128 + WTERMSIG(status);
            
            // عرض رسالة مناسبة عندما تقتل العملية بواسطة إشارة
            if (WTERMSIG(status) == SIGINT) {
                printf("\n");
            } else if (WTERMSIG(status) == SIGQUIT) {
                printf("Quit (core dumped)\n");
            }
        }
    }
}
```

## 3. مفاهيم متقدمة في إدارة العمليات

### 3.1. مجموعات العمليات والجلسات

- **مجموعة العمليات**: مجموعة من العمليات المرتبطة يمكن إدارتها معاً
- **الجلسة**: مجموعة من مجموعات العمليات، عادةً مرتبطة بجلسة terminal

```c
#include <unistd.h>

// الحصول على أو تعيين معرف مجموعة العمليات
pid_t getpgrp(void);
pid_t getpgid(pid_t pid);
int setpgid(pid_t pid, pid_t pgid);

// الحصول على أو تعيين معرف الجلسة
pid_t getsid(pid_t pid);
pid_t setsid(void);
```

### 3.2. أولوية العمليات

```c
#include <sys/resource.h>

// تعيين/الحصول على الأولوية
int getpriority(int which, id_t who);
int setpriority(int which, id_t who, int value);
```

### 3.3. عمليات الـ Daemon

```c
// إنشاء daemon
pid_t pid = fork();
if (pid > 0) exit(0);  // خروج الأب
setsid();              // إنشاء جلسة جديدة
// إعادة توجيه الإدخال/الإخراج، تغيير المجلد، إلخ
```

## 4. نصائح وإرشادات لمشروع Minishell

1. **ابدأ بتنفيذ الأوامر البسيطة**:
   - ابدأ بتنفيذ الأوامر بدون pipes/redirections.
   - تعلم كيفية تحليل الأوامر وتحويلها إلى مصفوفة argv.

2. **تعامل مع الأوامر المدمجة في shell**:
   - الأوامر المدمجة مثل cd, echo, pwd يجب تنفيذها مباشرة (بدون fork/exec).
   - تأكد من أن exit يخرج من الـ shell بشكل صحيح.

3. **تعامل مع البحث في PATH**:
   - استخدم getenv("PATH") للحصول على مسار البحث.
   - اختبر كل مسار لمعرفة ما إذا كان الملف موجوداً وقابل للتنفيذ.

4. **تعامل مع الإشارات بشكل صحيح**:
   - قم بإعداد معالجات الإشارات قبل الدخول في الحلقة الرئيسية.
   - تأكد من التعامل مع Ctrl+C, Ctrl+D, Ctrl+\ بشكل صحيح.

5. **ابحث عن تسربات الذاكرة**:
   - العمليات تنتهي، ولكن التسربات في العملية الأب (الـ shell) تتراكم.
   - استخدم valgrind لاكتشاف تسربات الذاكرة.

6. **تعامل مع حالة الخروج**:
   - احفظ حالة الخروج من آخر أمر تم تنفيذه لاستخدامها في $?.
   - تذكر أن الإشارات لها حالات خروج خاصة (128 + signal_number).

7. **اختبر الحالات الصعبة**:
   - أوامر غير موجودة.
   - أسماء ملفات غير صالحة.
   - إعادة توجيه إلى/من ملفات لا يمكن الوصول إليها.
   - التعامل مع علامات التنصيص المفردة والمزدوجة.

## 5. مثال شامل

### 5.1. حلقة رئيسية لـ Shell بسيط

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

#define BUFFER_SIZE 1024
#define MAX_ARGS 64

// متغيرات عامة
int last_exit_status = 0;
char *current_line = NULL;

// عرض الـ prompt
void display_prompt() {
    char cwd[BUFFER_SIZE];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s$ ", cwd);
    } else {
        printf("$ ");
    }
    fflush(stdout);
}

// تنفيذ أمر مدمج
int execute_builtin(char *args[]) {
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            // cd بدون وسائط يذهب إلى HOME
            char *home = getenv("HOME");
            if (home && chdir(home) != 0) {
                perror("cd");
            }
        } else if (chdir(args[1]) != 0) {
            perror("cd");
        }
        return 1;
    } else if (strcmp(args[0], "exit") == 0) {
        // تنفيذ exit
        int status = 0;
        if (args[1] != NULL) {
            status = atoi(args[1]);
        }
        exit(status);
    } else if (strcmp(args[0], "pwd") == 0) {
        // تنفيذ pwd
        char cwd[BUFFER_SIZE];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("pwd");
        }
        return 1;
    }
    
    // ليس أمراً مدمجاً
    return 0;
}

// معالج الإشارات
void signal_handler(int signum) {
    if (signum == SIGINT) {
        // Ctrl+C
        printf("\n");
        if (current_line) {
            free(current_line);
            current_line = NULL;
        }
        display_prompt();
    }
}

// إعداد معالجة الإشارات
void setup_signals() {
    struct sigaction sa;
    
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    sigaction(SIGINT, &sa, NULL);
    signal(SIGQUIT, SIG_IGN);
}

// تنفيذ أمر خارجي
void execute_external(char *args[]) {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
    } else if (pid == 0) {
        // العملية الوليدة
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        
        execvp(args[0], args);
        fprintf(stderr, "%s: command not found\n", args[0]);
        exit(EXIT_FAILURE);
    } else {
        // العملية الأب
        int status;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            last_exit_status = WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
            last_exit_status = 128 + WTERMSIG(status);
            if (WTERMSIG(status) == SIGINT) {
                printf("\n");
            }
        }
    }
}

int main() {
    char *input;
    size_t input_size = 0;
    
    // إعداد معالجة الإشارات
    setup_signals();
    
    while (1) {
        display_prompt();
        
        // قراءة الإدخال
        ssize_t read = getline(&input, &input_size, stdin);
        current_line = input;
        
        if (read == -1) {
            // EOF (Ctrl+D)
            printf("exit\n");
            break;
        }
        
        // إزالة newline
        if (read > 0 && input[read-1] == '\n') {
            input[read-1] = '\0';
        }
        
        // تجاهل الأسطر الفارغة
        if (strlen(input) == 0) {
            continue;
        }
        
        // تقسيم الإدخال إلى وسائط
        char *args[MAX_ARGS];
        int arg_count = 0;
        
        char *token = strtok(input, " \t");
        while (token != NULL && arg_count < MAX_ARGS - 1) {
            args[arg_count++] = token;
            token = strtok(NULL, " \t");
        }
        args[arg_count] = NULL;
        
        if (arg_count > 0) {
            // محاولة تنفيذ كأمر مدمج
            if (!execute_builtin(args)) {
                // إذا لم يكن أمراً مدمجاً، نفذه كأمر خارجي
                execute_external(args);
            }
        }
        
        current_line = NULL;
    }
    
    free(input);
    return last_exit_status;
}
```

## خلاصة

الفهم العميق لإدارة العمليات ضروري لبناء shell قوي وفعال. المفاهيم الرئيسية هي:

1. **إنشاء العمليات** باستخدام `fork()`
2. **تنفيذ البرامج** باستخدام عائلة `exec`
3. **انتظار العمليات** باستخدام `wait()` و `waitpid()`
4. **معالجة الإشارات** لـ Ctrl+C وإشارات أخرى
5. **إدارة حالات الخروج** للأوامر المنفذة

عند بناء Minishell الخاص بك، ستجمع هذه المفاهيم مع pipes والتوجيه والتحليل لتنفيذ shell متكامل.

</div>