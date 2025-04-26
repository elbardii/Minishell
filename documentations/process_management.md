
## 1. Definitions:
#### fork() :
    katkhla9 lina process jdid (child process) tib9 l2asl l'main process (parent precess) kay3tih copy dyal memory, file descriptor... bwahd technique smitha copy-On-Write, katreturni l child 0 o l'parent katreturni lih pid dyal child w -1 f 7alat tra chi mochkil, child kaykml mn lpoint flcode li wsal lih lparent fach 3yt l fork, btw rah makinach chi taratobiya binathom y3ni momkin child ykhdm lwl momkin parent ykhdm lwl, f7alat ma drty 3awtani fork mora lwla ght3awd tcree lina child process mn lparent wlkin maghtcreeyich wahd lchild li drna flwl.

#### wait() :
    ktkhali l'parent ytsna chi child process ymot, ktrj3 PID dyalo, katkhdm bach t7yd zombie processes, ila makin hta child process ktrj3 -1 w kat7t errno. katcontroli l'order dyal l'execution.

#### waitpid() :
    hadi bhal wait ghahowa blast mntsnaw ay child process ysali kntsnaw process bdabt ysali.

#### wait3/wait4 :
    bhal wait w waitpid gha homa ky3tiwk hta resource usage f struct rusage gha howa wait3 ktsna ay child process w wait4 t9dr t3tiha pid dyalo.

## 2. Understanding:

    ila bghina ncreeyiw wahd child process kn3yto 3la fork() li ktkhli lprocess ybda lkhdma mn wra lcall dyalha ktreturni lih 0 w ktreturni l parent pid dyal child, ila bghina ntsnawh n9dro n3yto 3la ay wait mn li chr7t f wahd if statement ila kan dak return mn ghir zero ydir wait binma sala lprocess dyal child.

## 3. execve() :

    hiya system call f Linux/Unix li katkhali l'process li khddam daba ybdl rasso b programme jdid. Katms7 koulchi li kayn f la mémoire dyal lprocess l9dim (mn code, data, stack...), w katsayb plasso l-programme jdida li kat3tiha (pathname), m3a l-arguments (argv) w les variables dyal l'environment (envp). Mlli execve() katnaja7, l'process l9dim kaytna9a w l'programme jdid kaykhdem b nafs l'PID. execve() makatrd lik 7ta haja ila khdmat w errno ila tra chi mochkil, li kay3ni bli l'execution katsift l'massar jdid bla ma katsali l'function. Lmzyan f execve() howa f fork() w execve() m3a ba3dhiyathom: katdir fork() bach tkhl9 process jdida, w dak process (weld) kaydir execve() bach ybddl rasso b programme jdid, bla may2tr 3la lprocess l2asli (parent)
