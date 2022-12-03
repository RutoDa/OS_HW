#include <stdlib.h> 
#include <unistd.h>
#include <errno.h> 
#include <stdio.h> 
#include <string.h>
#include <sys/wait.h>

void input_split(char *input_str, char *path)  {
    // 用迴圈方式拆解 input string 的路徑與 ＆
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
    char is_back_ground = 0;
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
            is_back_ground = 0;
            
            if (input_str[strlen(input_str)-2]=='&') is_back_ground = 1;

            input_split(input_str, path);
        
            if (is_back_ground == 1) {
                // parent 不會 wait child
                pid = fork(); // 產生child
                if (pid == 0) {
                    execl(path,"child",NULL);
                    exit(1);
                } 
            } else {
                // parent 會 wait child
                pid = fork(); // 產生child
                if (pid == 0) {
                    execl(path,"child",NULL);
                    printf("%s is not found. fork() succeeded\n", path);
                    exit(1);
                } else if (pid > 0) {
                    pid_t ch = wait(&child_exit_status);
                    //printf("\nI'm parent. waitpid %d, status %d\n", pid, child_exit_status);
                } else {
                    printf("%s is not found. fork() succeeded\n", path);
                    exit(1);
                }
            }   
        }
    }
    return 0;
}
