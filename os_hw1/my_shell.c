#include <stdlib.h> 
#include <unistd.h>
#include <errno.h> 
#include <stdio.h> 
#include <string.h>
#include <sys/wait.h>

void input_split(char *input_str, char *path)  {
    // 用迴圈方式將 input string 中的路徑 存放到 path
    for (int i = 0; i < strlen(input_str); i++) {
        if (input_str[i] == ' ') { 
            // 假如讀到空格表示 input string 的 path 已結束
            path[i] = '\0'; // 將 path 最後一格填上 \0 變為字串
            break;
        } else if (input_str[i] == '\n') { 
            // 假如讀到 \n 表示 input string 整個讀完了
            path[i] = '\0'; // 將 path 最後一格填上 \0 變為字串
        } else {
            // 將 path index i 的 element 填上 input 的 i 的 element
            path[i] = input_str[i];
        }
    }
}

int main () {
    char no_wait = 0;
    pid_t parent_pid = getpid();
    pid_t pid;
    printf("Parent pid is %d\n", parent_pid);
    for(;;) {
        char path[1025]; // store path，linux 路徑最長限制為 1024byte + 字串最後一格填 \0
        char input_str[1027]; // store input string，比 path 多 2byte 來存 &
        int child_exit_status;
        printf("1094841 ms> "); // print 提示符號
        
        if (fgets(input_str, sizeof(input_str), stdin)) 
        {
            no_wait = 0; // 先設為 0
            if (input_str[strlen(input_str)-2]=='&') {
                //假如 input string 的最後一個字為 &
                no_wait = 1; // 表示 my_shell 不需等待 chlild
            }
        
            // 將 input string 中的路徑 存放到 path
            input_split(input_str, path); 
        
            if (no_wait == 1) {
                // my_shell 不會 wait child
                pid = fork(); // 產生child
                if (pid == 0) {
                    // child 
                    execl(path,path,NULL); // 將 child 改成指定路徑的程式碼
                    printf("%s is not found. fork() succeeded\n", path);
                    exit(1); // execl 失敗就會中止 child
                } 
            } else {
                // my_shell 會 wait child
                pid = fork(); // 產生child

                if (pid == 0) {
                    // parent 會進來
                    execl(path,path,NULL);
                    printf("%s is not found. fork() succeeded\n", path);
                    exit(1); // execl 失敗就會中止 child
                } else if (pid > 0) {
                    pid_t ch = wait(&child_exit_status);
                    printf("\nI'm parent. waitpid %d, status %d\n", pid, child_exit_status);
                } else {
                    printf("%s is not found. fork() succeeded\n", path);
                    exit(1);
                }
            }   
        }
    }
    return 0;
}
