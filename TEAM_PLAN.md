# Team Task Division Plan for Minishell Project

<div align="center">
  <a href="#team-task-division-plan"><img src="https://img.shields.io/badge/Read%20in-English-blue?style=for-the-badge" alt="English"></a>
  <a href="#خطة-تقسيم-المهام-للفريق"><img src="https://img.shields.io/badge/Darija -green?style=for-the-badge" alt="Moroccan Darija"></a>
</div>

## Team Task Division Plan

**Planning Date: April 24, 2025**

### Main Task Distribution:

## 1. Phase One: Study and Preparation (One Week)

#### @yatanagh:
- **Detailed study of Process Management:**
  * Documentation of fork(), wait(), waitpid(), wait3(), wait4()
  * Understanding how to create child processes and wait for them
  * Exploring execve() and how to replace processes
  * Create markdown documentation in `/documentations/process_management.md`

- **Study of Signals and their management:**
  * signal(), sigaction(), sigemptyset(), sigaddset(), kill()
  * How to handle ctrl-C, ctrl-D, and ctrl-\
  * Create markdown documentation in `/documentations/signal_management.md`

#### @isel-bar:
- **Detailed study of File Descriptors and Redirections:**
  * pipe(), dup(), dup2()
  * open(), close(), read(), write()
  * Understanding how < > << >> work
  * Create markdown documentation in `/documentations/file_descriptors.md`

- **Study of Command Line Parsing:**
  * How to analyze user input text
  * Handling quotation marks (' and ")
  * Study of readline() and add_history()
  * Create markdown documentation in `/documentations/command_parsing.md`

## 2. Phase Two: Project Basic Structure (One Week)

#### @yatanagh:
- **Creating the project's basic structure:**
  * Setting up Makefile
  * Creating header files
  * Organizing folder structure
  * Linking libft

- **Programming the Main Loop:**
  * Displaying the prompt
  * Reading inputs using readline
  * Managing history

#### @isel-bar:
- **Implementing the Basic Parser:**
  * Splitting command line into tokens
  * Identifying commands and parameters
  * Handling single and double quotation marks

- **Creating basic data structures:**
  * Structuring storage for commands and their parameters
  * Representing redirections and pipes

## 3. Phase Three: Implementing Built-in Commands (3-4 Days)

#### @yatanagh:
- **Programming the following commands:**
  * cd (navigating between directories)
  * pwd (displaying current path)
  * exit (exiting the shell)

- **Basic environment variable management:**
  * Storing current environment variables
  * Reading $? value

#### @isel-bar:
- **Programming the following commands:**
  * echo with -n option
  * export (exporting variables)
  * unset (removing variables)
  * env (displaying environment variables)

- **Implementing $ processing:**
  * Expanding environment variables ($VAR)
  * Managing $? (exit status)

## 4. Phase Four: External Execution and Redirections (One Week)

#### @yatanagh:
- **Programming external program execution:**
  * Searching for executable files using PATH
  * Creating child processes using fork
  * Executing programs using execve
  * Waiting for processes to complete

- **Implementing pipes (|) mechanism:**
  * Connecting outputs of one command to inputs of another
  * Managing a series of sequential commands

#### @isel-bar:
- **Implementing redirections:**
  * < (input redirection)
  * > (output redirection)
  * << (heredoc)
  * >> (append output redirection)

- **File Descriptor management:**
  * Saving and redirecting standard input and output
  * Closing unused File Descriptors

## 5. Phase Five: Signal Management and Testing (One Week)

#### Together as a team (@yatanagh & @isel-bar):
- **Implementing signal management:**
  * Handling ctrl-C (displaying new prompt)
  * Handling ctrl-D (exiting the shell)
  * Handling ctrl-\ (doing nothing)

- **Comprehensive testing and debugging:**
  * Testing all mandatory features
  * Ensuring no memory leaks
  * Checking norm compliance

## 6. Phase Six: Bonus Parts (One Additional Week)

#### @yatanagh:
- **Implementing && and || with parentheses for priorities:**
  * Updating Parser to support logical operations
  * Implementing mechanism for handling priorities
  * Testing complex cases

#### @isel-bar:
- **Implementing Wildcards (*) support:**
  * Searching for matching files in current directory
  * Integrating results with commands
  * Testing various cases

## Meeting and Follow-up Schedule:

1. **Weekly meeting** for review and planning (at least once a week)
2. **Short daily meeting** for 15 minutes to discuss progress and challenges
3. **Code review** after each major phase
4. **Joint testing session** before project submission

## Additional Tips:

1. **Using Git** effectively:
   * Creating branches for each feature
   * Making small and regular commits
   * Using pull requests for review

2. **Documenting code** while writing:
   * Comments clarify complex parts
   * Explaining logic behind important decisions

3. **Creating tests** for each part:
   * Tests for each built-in command
   * Tests for the parser
   * Tests for redirections and pipes

4. **Parallel work**:
   * Coordination to avoid code conflicts
   * Setting clear interfaces between modules

## Suggested Learning Path (Links and Resources):

1. **For Processes and Signals**:
   * Simplified exercises on fork(), exec(), and wait()
   * Review examples of signal handling

2. **For File Descriptors and Redirections**:
   * Exercises on pipe() and dup2()
   * Examples of input and output redirection

3. **For Parser**:
   * Analyzing how commands are parsed in Bash
   * Trying to build a simple parser

---

# خطة تقسيم المهام للفريق

**تاريخ التخطيط: 24 أبريل 2025**

### توزيع المهام الرئيسية:

## 1. المرحلة الأولى: الدراسة والتحضير (أسبوع واحد)

#### @yatanagh:
- **دراسة مفصلة لإدارة العمليات:**
  * توثيق fork(), wait(), waitpid(), wait3(), wait4()
  * فهم كيفية إنشاء عمليات فرعية والانتظار ديالها
  * استكشاف execve() وكيفية استبدال العمليات
  * إنشاء توثيق في ملف `/documentations/process_management.md`

- **دراسة الإشارات (Signals) وإدارتها:**
  * signal(), sigaction(), sigemptyset(), sigaddset(), kill()
  * كيفية التعامل مع ctrl-C, ctrl-D و ctrl-\
  * إنشاء توثيق في ملف `/documentations/signal_management.md`

#### @isel-bar:
- **دراسة مفصلة لواصفات الملفات وإعادة التوجيه:**
  * pipe(), dup(), dup2()
  * open(), close(), read(), write()
  * فهم كيفية عمل < > << >>
  * إنشاء توثيق في ملف `/documentations/file_descriptors.md`

- **دراسة تحليل سطر الأوامر:**
  * كيفية تحليل النص اللي دخلو المستخدم
  * التعامل مع علامات التنصيص (' و ")
  * دراسة readline() و add_history()
  * إنشاء توثيق في ملف `/documentations/command_parsing.md`

## 2. المرحلة الثانية: البنية الأساسية للمشروع (أسبوع واحد)

#### @yatanagh:
- **إنشاء الهيكل الأساسي للمشروع:**
  * إعداد Makefile
  * إنشاء ملفات header
  * تنظيم بنية المجلدات
  * ربط libft

- **برمجة الحلقة الرئيسية:**
  * عرض الـ prompt
  * قراءة المدخلات باستخدام readline
  * إدارة التاريخ (history)

#### @isel-bar:
- **تنفيذ المحلل الأساسي:**
  * تقسيم سطر الأوامر إلى tokens
  * التعرف على الأوامر والوسائط
  * التعامل مع علامات التنصيص المفردة والمزدوجة

- **إنشاء هياكل البيانات الأساسية:**
  * هيكلة تخزين الأوامر ووسائطها
  * تمثيل إعادة التوجيه والأنابيب

## 3. المرحلة الثالثة: تنفيذ الأوامر المدمجة (3-4 أيام)

#### @yatanagh:
- **برمجة الأوامر التالية:**
  * cd (التنقل بين المجلدات)
  * pwd (عرض المسار الحالي)
  * exit (الخروج من الـshell)

- **إدارة متغيرات البيئة الأساسية:**
  * تخزين متغيرات البيئة الحالية
  * قراءة قيمة $?

#### @isel-bar:
- **برمجة الأوامر التالية:**
  * echo مع خيار -n
  * export (تصدير المتغيرات)
  * unset (إلغاء المتغيرات)
  * env (عرض متغيرات البيئة)

- **تنفيذ معالجة $:**
  * توسيع متغيرات البيئة ($VAR)
  * إدارة $? (حالة الخروج)

## 4. المرحلة الرابعة: التنفيذ الخارجي وإعادة التوجيه (أسبوع واحد)

#### @yatanagh:
- **برمجة تنفيذ البرامج الخارجية:**
  * البحث عن الملفات التنفيذية باستخدام PATH
  * إنشاء عمليات فرعية باستخدام fork
  * تنفيذ البرامج باستخدام execve
  * انتظار انتهاء العمليات

- **تنفيذ آلية الأنابيب (|):**
  * ربط مخرجات أمر بمدخلات أمر آخر
  * إدارة مجموعة من الأوامر المتسلسلة

#### @isel-bar:
- **تنفيذ إعادة التوجيه:**
  * < (توجيه المدخلات)
  * > (توجيه المخرجات)
  * << (heredoc)
  * >> (توجيه المخرجات بالإضافة)

- **إدارة واصفات الملفات:**
  * حفظ وإعادة توجيه المدخلات والمخرجات القياسية
  * إغلاق واصفات الملفات غير المستخدمة

## 5. المرحلة الخامسة: إدارة الإشارات والاختبار (أسبوع واحد)

#### معا كفريق (@yatanagh & @isel-bar):
- **تنفيذ إدارة الإشارات:**
  * التعامل مع ctrl-C (عرض prompt جديد)
  * التعامل مع ctrl-D (الخروج من الـshell)
  * التعامل مع ctrl-\ (عدم فعل شيء)

- **اختبار شامل وتصحيح الأخطاء:**
  * اختبار جميع الميزات الإلزامية
  * التأكد من عدم وجود تسريب للذاكرة
  * فحص الامتثال للـnorm

## 6. المرحلة السادسة: الأجزاء الإضافية (أسبوع واحد إضافي)

#### @yatanagh:
- **تنفيذ && و || مع الأقواس للأولويات:**
  * تحديث المحلل لدعم العمليات المنطقية
  * تنفيذ آلية للتعامل مع الأولويات
  * اختبار الحالات المعقدة

#### @isel-bar:
- **تنفيذ دعم الرموز العامة (*):**
  * البحث عن الملفات المطابقة في المجلد الحالي
  * دمج النتائج مع الأوامر
  * اختبار الحالات المختلفة

## جدول الاجتماعات والمتابعة:

1. **اجتماع أسبوعي** للمراجعة والتخطيط (مرة واحدة على الأقل في الأسبوع)
2. **اجتماع يومي قصير** لمدة 15 دقيقة لمناقشة التقدم والتحديات
3. **مراجعة الكود** بعد كل مرحلة رئيسية
4. **جلسة اختبار مشتركة** قبل تسليم المشروع

## نصائح إضافية:

1. **استخدام Git** بشكل فعال:
   * إنشاء فروع (branches) لكل ميزة
   * عمل commits صغيرة ومنتظمة
   * استخدام pull requests للمراجعة

2. **توثيق الكود** أثناء الكتابة:
   * التعليقات توضح الأجزاء المعقدة
   * شرح المنطق وراء القرارات المهمة

3. **إنشاء اختبارات** لكل جزء:
   * اختبارات لكل أمر مدمج
   * اختبارات للمحلل
   * اختبارات لإعادة التوجيه والأنابيب

4. **العمل المتوازي**:
   * التنسيق لتجنب تضارب الكود
   * وضع واجهات واضحة بين الوحدات

## مسار التعلم المقترح (موارد):

1. **للعمليات والإشارات**:
   * تمارين مبسطة على fork(), exec() و wait()
   * مراجعة أمثلة على التعامل مع الإشارات

2. **لواصفات الملفات وإعادة التوجيه**:
   * تمارين على pipe() و dup2()
   * أمثلة على توجيه المدخلات والمخرجات

3. **للمحلل**:
   * تحليل كيفية تحليل الأوامر في Bash
   * تجربة بناء محلل بسيط