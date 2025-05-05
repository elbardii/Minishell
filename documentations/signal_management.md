### 📚 شرح الإشارات (Signals) بلغة C - بالدارجة المغربية

---

### 🛠 `signal()`

&#x20;دالة كتستعمل باش تربط واحد signal مع دالة (callback) اللي كتسمّى signal handler. منين البرنامج كيستقبل داك signal، كينفّذ داك handler عوض ما يسالي مباشرة

#### ✅ الشكل العام:

```c
void (*signal(int signum, void (*handler)(int)))(int);
```

* `signum`: رقم الإشارة بحال `SIGINT`.
* `handler`: هي دالة كتكتبها انت، فيها الكود اللي يتنفذ ملي توصل signal.

#### ✅ مثال:

```c
#include <stdio.h>
#include <signal.h>

void my_handler(int sig) {
    printf("توصلت بـ signal رقم %d\n", sig);
}

int main() {
    signal(SIGINT, my_handler);

    while (1) {
        printf("خدام...\n");
        sleep(1);
    }
    return 0;
}
```

📌 إيلا المستخدم دار Ctrl-C، البرنامج غادي يطبع رسالة وما يساليش.

---

### 🛠 `sigaction()`

دالة كتعطيك تحكم كثر فالتعامل مع الإشارات، وتقدر من خلالها تحدد شنو إشارات كتتبلوكا مؤقتًا.

#### ✅ الشكل:

```c
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
```

* `signum`: رقم الإشارة.
* `act`: struct فيها التفاصيل.
* `oldact`: تقدر تخزن فيها الإعدادات القديمة.

#### ✅ أهم الحقول:

```c
struct sigaction {
    void     (*sa_handler)(int);   // الدالة اللي غادي تنفذ
    sigset_t  sa_mask;             // إشارات كتتبلوكا مؤقتًا
    int       sa_flags;            // خصائص إضافية
};
```

#### ✅ مثال:

```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void my_handler(int sig) {
    write(1, "✨ توصلت بـ signal\n", 23);
}

int main() {
    struct sigaction sa;
    sa.sa_handler = my_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);

    while (1) pause();
    return 0;
}
```

📌 مفيدة بزاف فحالات multi-threading ولا ملي كتحتاج تتحكم فالإشارات بشكل دقيق.

---

### 🛠 `sigemptyset()`

كتفرغ واحد المجموعة ديال الإشارات (signal set)، يعني كتبداها من الزيرو.

```c
sigset_t set;
sigemptyset(&set);
```

📌 ضروري تستعملها قبل `sigaddset()` باش تكون متأكد المجموعة خاوية.

---

### 🛠 `sigaddset()`

كتزيد إشارة فـ signal set باش تستعملها من بعد فـ `sigaction()` ولا `sigprocmask()`.

```c
sigaddset(&set, SIGINT);
```

📌 مثلا تستعملها باش تبلوكي SIGINT مؤقتًا.

---

### 🛠 `kill()`

كتستعمل باش تبعت signal لشي process (برنامج) باستعمال PID ديالو.

```c
kill(pid, SIGINT);      // بعت signal لشي process
kill(getpid(), SIGINT); // بعت signal لنفسك
```

📌 `kill` ماشي معناها تقتل البرنامج، ولكن تبعت أي signal.

---

### ⌨️ تأثير Ctrl-C و Ctrl-D و Ctrl-\ على البرنامج

* **Ctrl-C**: كيبعت `SIGINT` (رقم 2). كيسالي البرنامج إلا كنت داير handler.
* \*\*Ctrl-\*\*: كيبعت `SIGQUIT` (رقم 3). بحالو بحال SIGINT ولكن كيخلي trace فـ core dump.
* **Ctrl-D**: ماشي signal. كيعني نهاية المعطيات (EOF) فـ stdin، بحال إيلا سديتي input.

---

### 💻 مثال عملي بكود: `sigaction()` بلا `signal()`

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handle_sigint(int sig) {
    printf("\n🚨 SIGINT توصّلنا بها (Ctrl-C). البرنامج مزال خدام.\n");
}

void handle_sigquit(int sig) {
    printf("\n🚨 SIGQUIT توصّلنا بها (Ctrl-\\). البرنامج غادي يسالي.\n");
    exit(0);
}

int main() {
    struct sigaction sa_int;
    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);

    struct sigaction sa_quit;
    sa_quit.sa_handler = handle_sigquit;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    sigaction(SIGQUIT, &sa_quit, NULL);

    printf("🔁 البرنامج خدام... جرب Ctrl-C أو Ctrl-\\ باش تشوف التأثير.\n");

    while (1) {
        pause();
    }
    return 0;
}
```
