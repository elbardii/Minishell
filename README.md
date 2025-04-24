# Minishell

## Project Overview

Minishell is a simplified shell implementation project that mimics basic functionalities of bash. This project challenges you to create a command-line interpreter from scratch, providing deep insights into how shells work, process management, and system calls in Unix-based operating systems.

## Technical Requirements

### Mandatory Requirements

1. **Programming Language**: 
   - The entire project must be written in C
   - Code must comply with the 42 School Norm

2. **Memory Management**:
   - All allocated memory must be properly freed
   - No memory leaks are permitted
   - Use of appropriate memory debugging tools is recommended

3. **Build System**:
   - A Makefile with at least the following rules: `NAME`, `all`, `clean`, `fclean`, and `re`
   - Compilation must use flags: `-Wall`, `-Wextra`, and `-Werror`

4. **Error Handling**:
   - Your program should handle errors gracefully
   - All error messages should be clear and informative

### Functional Requirements

Your shell must:

1. **Display a prompt** when waiting for a new command
2. **Maintain a command history** that persists between sessions
3. **Search and launch executables** based on the PATH variable or using relative/absolute paths
4. **Use only one global variable** for signal handling that stores only the signal number
5. **Handle quotes appropriately**:
   - Single quotes (`'`) should prevent interpretation of all special characters
   - Double quotes (`"`) should prevent interpretation of all special characters except `$`
6. **Implement redirections**:
   - `<` : Input redirection
   - `>` : Output redirection
   - `<<` : Here document (read input until a line containing the delimiter is seen)
   - `>>` : Append output redirection
7. **Implement pipes** (`|`) to connect command outputs to command inputs
8. **Handle environment variables** (variables preceded by `$`)
9. **Handle `$?`** which expands to the exit status of the most recently executed foreground command
10. **Handle signals** like in bash:
    - `Ctrl-C`: Display a new prompt
    - `Ctrl-D`: Exit the shell
    - `Ctrl-\`: Do nothing
11. **Implement built-in commands**:
    - `echo` with option `-n`
    - `cd` with relative or absolute path
    - `pwd` without options
    - `export` without options
    - `unset` without options
    - `env` without options or arguments
    - `exit` without options

### Bonus Features

If you complete all mandatory requirements perfectly, you can implement:

1. **Logical operators** (`&&` and `||`) with parentheses for priorities
2. **Wildcards** (`*`) for the current working directory

## Core Concepts

### 1. Process Management
- Understanding process creation using `fork()`
- Managing parent and child processes
- Executing commands with `execve()`
- Waiting for processes to complete with `wait()` family functions

### 2. File Descriptors
- Managing input/output streams
- Redirecting standard input/output/error
- Handling pipe communication between processes
- Understanding file opening, reading, writing, and closing

### 3. Signal Handling
- Intercepting keyboard signals (SIGINT, SIGQUIT)
- Proper signal handling to avoid zombie processes
- Managing signals across multiple processes

### 4. Command Parsing
- Tokenizing and parsing command lines
- Handling special characters and quotes
- Building an abstract syntax tree for complex commands

### 5. Environment Variables
- Managing environment variables
- Expanding variables in commands
- Implementing built-in commands that modify the environment

## Prerequisites Knowledge

Before starting this project, ensure you understand:

1. **C Programming**: Particularly memory management and pointers
2. **System Calls**: `fork()`, `execve()`, `wait()`, `pipe()`, `dup()/dup2()`
3. **File I/O**: Reading from and writing to files using file descriptors
4. **Signal Handling**: How to catch and handle signals in C
5. **Parsing Algorithms**: How to properly parse command lines
6. **Libft**: Your 42 custom library if it's validated

## Development Approach

1. **Modular Design**: Break down the project into smaller, manageable components
2. **Incremental Implementation**: Start with basic functionality and gradually add features
3. **Test-Driven Development**: Test each component thoroughly before moving on
4. **Memory Management**: Use tools like Valgrind to check for memory leaks
5. **Documentation**: Document your code and design decisions

## Project Structure Suggestion

```
minishell/
├── include/             # Header files
├── src/
│   ├── builtins/        # Built-in command implementations
│   ├── execution/       # Command execution logic
│   ├── parsing/         # Command line parsing
│   ├── signals/         # Signal handling
│   ├── utils/           # Utility functions
│   └── main.c           # Entry point
├── Makefile
└── README.md
```

## Tips for Success

1. **Start Simple**: Begin with a basic shell that can execute simple commands
2. **Test Regularly**: Test each feature as you implement it
3. **Use Debugging Tools**: Valgrind for memory leaks, GDB for debugging
4. **Read Man Pages**: Understand the system calls you're using
5. **Collaborate**: Discuss your approach with peers to gain new insights
6. **Plan Before Coding**: Design your data structures and algorithms before implementation

## Resources

- Unix Programming manuals (man pages)
- "Advanced Programming in the UNIX Environment" by W. Richard Stevens
- Bash Reference Manual
- The GNU C Library documentation

---

*Note: This project is an important milestone in understanding operating system concepts, process management, and system programming. While challenging, it provides invaluable insights into how shells work and interact with the operating system.*


المشروع بالدارجة المغربية:


## شنو هو هاد المشروع؟

هاد المشروع كيطلب منك تخلق واحد "shell" بسيط، يعني نظام تحكم بالحاسوب شبيه ل "Bash". فاش غادي تخدم هاد المشروع، غادي تكتسب معرفة عميقة حول "processes" و "file descriptors" اللي هوما من المفاهيم الأساسية فعالم البرمجة.

## المتطلبات التقنية

### المتطلبات الإلزامية

1. **لغة البرمجة**:
   - المشروع كامل خاصو يكون مكتوب بلغة C
   - الكود خاصو يتبع معايير مدرسة 42

2. **إدارة الذاكرة**:
   - جميع الذاكرة المخصصة خاصها تتحرر بشكل صحيح
   - ماخاصش يكونو تسريبات الذاكرة
   - استخدام أدوات تصحيح الذاكرة الموصى بها

3. **نظام البناء**:
   - ملف Makefile خاصو يكون فيه على الأقل القواعد التالية: `NAME`, `all`, `clean`, `fclean`, و `re`
   - التجميع خاصو يستخدم العلامات: `-Wall`, `-Wextra`, و `-Werror`

4. **معالجة الأخطاء**:
   - البرنامج ديالك خاصو يتعامل مع الأخطاء بشكل جيد
   - جميع رسائل الخطأ خاصها تكون واضحة ومفهومة

### المتطلبات الوظيفية

الـ shell ديالك خاصو:

1. **يعرض موجه الأوامر** ملي كيتسنى أمر جديد
2. **يحافظ على تاريخ الأوامر** اللي كيبقى بين الجلسات
3. **يبحث ويشغل الملفات التنفيذية** بناءً على متغير PATH أو باستخدام المسارات النسبية/المطلقة
4. **يستخدم غير متغير عالمي واحد** لمعالجة الإشارات اللي كيخزن غير رقم الإشارة
5. **يتعامل مع علامات التنصيص بشكل صحيح**:
   - علامات التنصيص الفردية (`'`) خاصها تمنع تفسير جميع الرموز الخاصة
   - علامات التنصيص المزدوجة (`"`) خاصها تمنع تفسير جميع الرموز الخاصة ما عدا `$`
6. **تنفيذ إعادة التوجيه**:
   - `<` : إعادة توجيه الإدخال
   - `>` : إعادة توجيه الإخراج
   - `<<` : مستند هنا (قراءة الإدخال حتى يتم رؤية سطر يحتوي على الفاصل)
   - `>>` : إعادة توجيه الإخراج للإلحاق
7. **تنفيذ الأنابيب** (`|`) لربط مخرجات الأوامر بإدخالات الأوامر
8. **التعامل مع متغيرات البيئة** (المتغيرات المسبوقة بـ `$`)
9. **التعامل مع `$?`** اللي كيتوسع لحالة الخروج من آخر أمر تم تنفيذه في الواجهة الأمامية
10. **التعامل مع الإشارات** بحال فـ bash:
    - `Ctrl-C`: عرض موجه أوامر جديد
    - `Ctrl-D`: الخروج من الـ shell
    - `Ctrl-\`: مايدير والو
11. **تنفيذ الأوامر المدمجة**:
    - `echo` مع الخيار `-n`
    - `cd` مع المسار النسبي أو المطلق
    - `pwd` بلا خيارات
    - `export` بلا خيارات
    - `unset` بلا خيارات
    - `env` بلا خيارات أو حجج
    - `exit` بلا خيارات

### الميزات الإضافية

إلا كملتي جميع المتطلبات الإلزامية بشكل مثالي، تقدر تنفذ:

1. **المعاملات المنطقية** (`&&` و `||`) مع الأقواس للأولويات
2. **الرموز العامة** (`*`) للدليل الحالي

## المفاهيم الأساسية

### 1. إدارة العمليات (Process Management)
- فهم إنشاء العمليات باستخدام `fork()`
- إدارة العمليات الأم والعمليات الفرعية
- تنفيذ الأوامر باستخدام `execve()`
- الإنتظار حتى تكمل العمليات باستخدام عائلة دوال `wait()`

### 2. واصفات الملفات (File Descriptors)
- إدارة مسارات الإدخال والإخراج
- توجيه المدخلات والمخرجات القياسية
- التعامل مع التواصل عبر الأنابيب (pipes) بين العمليات
- فهم كيفية فتح وقراءة وكتابة وإغلاق الملفات

### 3. معالجة الإشارات (Signal Handling)
- اعتراض إشارات لوحة المفاتيح (SIGINT, SIGQUIT)
- معالجة الإشارات بشكل صحيح لتجنب العمليات المعلقة (zombie processes)
- إدارة الإشارات عبر عمليات متعددة

### 4. تحليل الأوامر (Command Parsing)
- تقسيم وتحليل سطور الأوامر
- التعامل مع الرموز الخاصة وعلامات التنصيص
- بناء شجرة تركيب مجردة للأوامر المعقدة

### 5. متغيرات البيئة (Environment Variables)
- إدارة متغيرات البيئة
- توسيع المتغيرات في الأوامر
- تنفيذ الأوامر المدمجة اللي كتعدل البيئة

## المعرفة اللازمة قبل البدء

قبل ما تبدا فهاد المشروع، تأكد أنك فاهم:

1. **برمجة C**: خصوصا إدارة الذاكرة والمؤشرات
2. **نداءات النظام**: `fork()`, `execve()`, `wait()`, `pipe()`, `dup()/dup2()`
3. **قراءة وكتابة الملفات**: القراءة والكتابة في الملفات باستخدام واصفات الملفات
4. **معالجة الإشارات**: كيفية التقاط ومعالجة الإشارات في C
5. **خوارزميات التحليل**: كيفية تحليل سطور الأوامر بشكل صحيح
6. **مكتبة Libft**: المكتبة الخاصة ديالك في 42 إلا كانت مقبولة

## طريقة التطوير

1. **تصميم معياري**: قسم المشروع لمكونات صغيرة يمكن التحكم فيها
2. **تنفيذ تدريجي**: بدا بالوظائف الأساسية وزيد الميزات بالتدريج
3. **تطوير موجه بالاختبارات**: اختبر كل مكون بشكل جيد قبل ما تنتقل للمكون التالي
4. **إدارة الذاكرة**: استخدم أدوات بحال Valgrind باش تتحقق من تسريبات الذاكرة
5. **التوثيق**: وثق الكود ديالك وقرارات التصميم

## اقتراح هيكل المشروع

```
minishell/
├── include/             # ملفات الرأس
├── src/
│   ├── builtins/        # تنفيذ الأوامر المدمجة
│   ├── execution/       # منطق تنفيذ الأوامر
│   ├── parsing/         # تحليل سطر الأوامر
│   ├── signals/         # معالجة الإشارات
│   ├── utils/           # دوال مساعدة
│   └── main.c           # نقطة الدخول
├── Makefile
└── README.md
```

## نصائح للنجاح

1. **بدا بسيط**: بدا بـ shell بسيط اللي كيقدر ينفذ أوامر بسيطة
2. **اختبر بانتظام**: اختبر كل ميزة ملي تنفذها
3. **استخدم أدوات التصحيح**: Valgrind لتسريبات الذاكرة، GDB للتصحيح
4. **اقرأ صفحات الدليل**: فهم نداءات النظام اللي كتستخدمها
5. **تعاون**: ناقش النهج ديالك مع الزملاء باش تكتسب رؤى جديدة
6. **خطط قبل البرمجة**: صمم هياكل البيانات والخوارزميات قبل التنفيذ

## الموارد

- أدلة برمجة Unix (صفحات الدليل)
- "Advanced Programming in the UNIX Environment" بواسطة W. Richard Stevens
- دليل مرجعي Bash
- وثائق مكتبة GNU C

---

*ملاحظة: هاد المشروع هو محطة مهمة لفهم مفاهيم نظام التشغيل، إدارة العمليات، وبرمجة النظام. رغم أنه تحدي، كيقدم رؤى قيمة حول كيفية عمل الـ shells والتفاعل مع نظام التشغيل.*