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