# خطة الخدمة ديال مشروع **Minishell**


| الـمحور                | الحوايج اللّي خاصّين بيه                                                        | شكون غادي يخدم                                 | الـOutput اللّي خصّو يخرّج                                | ملاحظات ديال الدمج والـTesting                             |   |                                      |
| ---------------------- | ------------------------------------------------------------------------------- | ---------------------------------------------- | --------------------------------------------------------- | ---------------------------------------------------------- | - | ------------------------------------ |
| **Infra ديال البروجي** | • `Makefile` كامل<br>• Structure ديال الدوسيات (`src/`, `include/`, `libft/` …) | *isel‑bar*                                     | أوّل نسخة ديال `Makefile` + سكريبتات مساعدة               | ضروري تدير Rules ديال `all / clean / fclean / re` من اللول |   |                                      |
| **Lexer / Tokenizer**  | • تقطيع السطر لـ `tokens`<br>• التعامل مع `quotes` و `metacharacters`           | *yatanagh*                                     | `lexer.c` + Unit tests بسيطين                             | ضروري تتّفقو على `struct token` قبل ما تبداو               |   |                                      |
| **Parser / AST**       | • بناء `AST` بسيطة<br>• تعرف على `pipes` و `redirections`                       | *isel‑bar*                                     | `parser.c/h` + Dump ديال الـAST للتست                     | كيستقبل `tokens` من yatanagh                               |   |                                      |
| **Executor**           | • `fork/execve`<br>• تجهيز `stdin/stdout` حسب `redirection` و `pipes`           | خدمة مشتركة (البداية عند *yatanagh*)           | `exec.c` + دعم أمر خارجي واحد عاد توسعو                   | تستو بأوامر بصح مع `valgrind`                              |   |                                      |
| **Built‑ins**          | الأوامر: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`                  | *isel‑bar*                                     | `builtins.c/h` + Unit test لكل أمر                        | تأكد بلي `redirection` خدام حتى فالـbuilt‑ins              |   |                                      |
| **Environment Vars**   | • نسخ `environ[]` ل `hash‑table` ولا `list`<br>• دعم `$VAR` و `$?`              | *yatanagh*                                     | `env.c/h` + API ديال `getenv/setenv/expand`               | هاد الـAPI غيحتاجوه بجوج                                   |   |                                      |
| **Signals**            | • `SIGINT` (`Ctrl-C`) و `EOF`<br>• Variable global وحيد                         | خدمة مشتركة (فالآخر)                           | `signals.c`                                               | خصكوم تختبرو فـ`readline()` وبراها                         |   |                                      |
| **Memory Management**  | • دوال `free_tree` و `free_tokens`<br>• مراقبة `leaks`                          | مسؤولية كل واحد فملفاتو                        | مايتدمج حتى يسالي `valgrind --leak-check=full` بلا Errors |                                                            |   |                                      |
| **Testing & CI**       | • سكريبت `tests.sh` كيدير حالات بزاف<br>• `GitHub Actions` لـ `make && tests`   | *isel‑bar* (يبني البنية) + مساهمات من yatanagh | `tests/` + `.github/workflows/ci.yml`                     | كل `push` كيصنّط الـCI                                     |   |                                      |
| **Bonus (اختياري)**    | • `&&`, \`                                                                      |                                                | `, `()`, `\*` (`wildcards\`)                              | يحدّد من بعد                                               | — | مايدخلوش فيه حتى يكمّلو الكور إلزامي |

---

## تقسيم الوقت (مثال ديال 3 سيمانات)

### السيمانة 1

* *isel‑bar*: يدير `Makefile` و Structure → يبدا `Parser`.
* *yatanagh*: يخدم `Lexer` + Plan ديال `env`.
* كل نهار ديرو Meeting صغيّر (Stand‑up) باش تتفقو على الـStructs.

### السيمانة 2

* دمجو `Lexer` مع `Parser`, و بداو `Executor` للأوامر الساهلين.
* بناء `built‑ins` الأساسيين (`echo`, `pwd`).
* حطّو Tests أوتوماتيكيين و شغّلو `valgrind`.

### السيمانة 3

* كمّلو `redirections` و `pipes`.
* ركّبو `signals` و صلّحو أي Leak.
* Review نهائي بجوج، إلا كان الوقت ديرو `bonus`.

---

## أحسن Practices

1. **Git Branching واضح**

   * `main` غير للكود المستقر.
   * استعملو فروع بحال `feature/lexer-yatanagh` ولا `feature/parser-iselbar`.
   * دمجو بـ Pull Request و Review ضرورية.

2. **API Contract ثابت**

   * وثّقو `struct Token` و `AST Node` فـ `README_DEV.md`.
   * أي تغيير خاصّو إخبار الطرف الآخر.

3. **Tools ديال Debug**

   * استعملو `valgrind`, `strace`, `bash -x` باش تقارنو السلوك.
   * حبسو الـTests فـ `tests/` باش تعاودو تشغلوهم بسهولة.

4. **اجتماعات خفيفة**

   * Stand‑up 15 دقيقة كل مساء لحل المشاكل قبل ما تكبر.

---

## نصائح خفيفة

* بْداو بأبسط Case: أمر واحد بلا `redirection` ولا `pipe`، وزيدو Step by Step.
* ماتديروش Over‑engineering؛ خدمو غير لي طالبين فـ Subject.
* Commentaires خفاف وواضحين فالـCode, وخليو `MANUAL.md` يشرح الاستعمال.
* Commits صغار وبعناوين واضحة: `parser: support pipes` مثلاً.
* جرّبو "minishell فقلب minishell" (Inception) تهناو من مفاجآت.


