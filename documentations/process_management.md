# System Calls: fork, wait, execve b Darija

## 1. Definitions:

Hado homa chi fonctions (system calls) mouhimmin f les systèmes bhal Linux/Unix:

#### `fork()`:

* **Ash kadir:** Katkhla9 process jdid (kaytsemma `child process`) li howa nouskha tib9 l2asl dyal l-process li 3iyyet liha (kaytsemma `parent process`).
* **Kifach:** Katpartaji m3ah la mémoire, les file descriptors, etc., b wa7ed technique smitha `Copy-On-Write` (kaydiro copy ghir melli chi we7ed ybghi ybeddel chi 7aja).
* **Ash katرجع:**
    * L l-`child process` katreturni `0`.
    * L l-`parent process` katreturni l `PID` (Process ID) dyal l `child`.
    * Katreturni `-1` f 7alat wqe3 chi mochkil (o kat7et sbab f `errno`).
* **L-Execution:** L `child` kaybda l-khdma mn nefs l-blassa f l-code li wsel liha l `parent` fach 3iyyet l `fork()`.
* **Taratobiya:** Makaynach taratobiya bin l `parent` o l `child`, y3ni momkin ay we7ed fihom ysbbe9 l-akhor f l-execution.
* **Multiple Forks:** Ila l `parent` 3awed dar `fork()` mera khera, ghaytkheli9 `child` jdid men dak l `parent`, machi men l `child` lewwel.

#### `wait()`:

* **Ash kadir:** Katkhalli l `parent process` yb9a ytsenna (block) 7talash chi wa7ed men lwlad dyalo (`child process`) ymout (ysali l-khdma dyalo).
* **Ash katرجع:**
    * Katرجع l `PID` dyal l `child` li mat.
    * Katرجع `-1` ila makan 3ndo 7ta `child` khddam, awla ila wqe3 chi mochkil (o kat7et `errno`).
* **Fayda:** Mouhimma bach t7iyed dakchi li kaytsemma `zombie processes` (les child li salaw walakin baqin kaytsennaw l parent yakhod l-information dyalhom) o bach tcontroli l-ordre dyal l'execution (matalan parent ytsenna child ysali 3ad ykemmel).

#### `waitpid()`:

* **Ash kadir:** B7al `wait()` تماماً, ghir howa kat3tiha l `PID` dyal `child` li bghitih ytsenna bdabt, machi ay `child` w safi. Kat9der tzid 7ta options khrin bach tcontroli kifach kattsenna.
* **Ash katرجع:** Katرجع l `PID` dyal dak l `child` li tsennetih, `0` (f 7alat l option `WNOHANG`), awla `-1` ila wqe3 mochkil (`errno`).

#### `wait3()` / `wait4()`:

* **Ash kadir:** B7al `wait()` (`wait3`) wla `waitpid()` (`wait4`) f l-principe dyal ttsenna chi `child` ysali.
* **Ziyada:** Kayzido 3la dok lkhrin annahom kayرجعo lik 7ta des informations 3la les resources li stahlek dak l `child` (CPU time, memory...) f wa7ed la structure smitha `struct rusage`.
    * `wait3()` katsenna ay `child`.
    * `wait4()` kat9der t3tiha `PID` mo7ddad dyal `child` li bghiti ttsenna.

## 2. Understanding (Kifach Nkhadmohom):

Melli nbghiw nkhadmo chi programme f process jdid bla manweqfo l-programme dyalna l-asli:

1.  **Kheli9 Process Jdid:** Kan3iyto 3la `fork()`. Hna kayweliw 3ndna jouj dyal les processes:
    * L `parent`: `fork()` katرجع lih l `PID` dyal l `child`.
    * L `child`: `fork()` katرجع lih `0`.
2.  **Tfari9 Lkhdma:** Kansta3mlo dik l valeur li rj3at men `fork()` bach nferqo lkhdma:
    * **F l-Code dyal l Parent (ila return machi 0):** Hna l parent 3aref rasso howa l parent. Ila bgha ytsenna l weld ysali, kay9der y3iyyet l chi fonction men `wait()` / `waitpid()` etc. Kayb9a ytsenna 7talash tرجع lih l function (m3naha l weld sala).
    * **F l-Code dyal l Child (ila return 0):** Hna l child 3aref rasso howa l child. Hna fin kaydir dakchi li bghina l child ydir, w f l-ghalib kay3iyyet l `execve()` (chofo section 3) bach ybeddel raso l programme we7dakhor.

```c
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        // Fork failed
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // *** Code dyal l Child Process ***
        printf("Ana l-child process (PID: %d)\n", getpid());
        // Hna fin l child kaydir execve() normalement
        // execlp("/bin/ls", "ls", "-l", NULL); // Example: run ls -l
        // perror("exec failed"); // Ila execve() rj3at, rah wqe3 mochkil
        // _exit(1); // Child kaykhroj ila exec failat
        printf("Child process kaykmmel...\n"); // Had l code ma ghaybanch ila exec nej7at
        return 0; // Child kaykhroj
    } else {
        // *** Code dyal l Parent Process ***
        printf("Ana l-parent process (PID: %d), kantsenna l child (PID: %d)\n", getpid(), pid);
        int status;
        waitpid(pid, &status, 0); // Parent kaytsenna l child bdabt ysali
        if (WIFEXITED(status)) {
            printf("Parent: L child sala b status %d\n", WEXITSTATUS(status));
        } else {
            printf("Parent: L child sala b tariqa ghayr tabi3iya\n");
        }
    }
    return 0;
}