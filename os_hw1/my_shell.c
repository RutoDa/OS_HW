#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

void input_split(char *input_str, char *path)
{
    // 用迴圈方式將 input string 中的路徑存放到 path
    for (int i = 0; i < strlen(input_str); i++)
    {
        if (input_str[i] == ' ' || input_str[i] == '&' || input_str[i] == '\n')
        {
            // 假如讀到空格或 \n 或 & 表示 input string 的 path 已結束
            path[i] = '\0'; // 將 path 最後一格填上 \0 變為字串
            break;
        }
        else
        {
            // 將 path index i 的 element 填上 input 的 i 的 element
            path[i] = input_str[i];
        }
    }
}

int main()
{
    char no_wait = 0;
    pid_t parent_pid = getpid();
    printf("Parent(my_shell) pid is %d\n", parent_pid);
    for (;;) // 跑無限迴圈
    {
        char path[1026];      // store path，linux 路徑最長限制為 1024byte + 字串最後一格填 \0
        char input_str[1028]; // store input string，比 path 多 3byte 來存 &\n
        int child_exit_status;
        printf("1094841 ms> "); // print 提示符號

        if (fgets(input_str, sizeof(input_str), stdin))
        {
            no_wait = 0; // 先設為 0
            if (input_str[strlen(input_str) - 2] == '&')
            {
                //假如 input string 的最後一個字為 &
                no_wait = 1; // 表示 my_shell 不需等待 chlild
            }

            // 將 input string 中的路徑 存放到 path
            input_split(input_str, path);
            if (strlen(path) == 0)
                continue;

            if (no_wait == 1)
            {
                // my_shell 不會 wait child
                pid_t pid = fork(); // 產生child
                if (pid == 0)
                {
                    // child 會跑以下程式碼
                    execl(path, path, NULL); // 將 child 改成指定路徑的程式碼
                    printf("%s is not found.\n", path);
                    exit(1); // execl 失敗就會中止 child
                }
                else if (pid > 0)
                {
                    // parent 什麼都不做
                }
                else
                {
                    // fork() 失敗
                    printf("fork() failed.\n");
                }
            }
            else
            {
                // my_shell 會 wait child
                pid_t pid = fork(); // 產生child

                if (pid == 0)
                {
                    // child 會跑以下程式碼
                    execl(path, path, NULL);
                    printf("%s is not found.\n", path);
                    exit(7); // execl 失敗就會中止 child ， 並回傳 7
                }
                else if (pid > 0)
                {
                    // parent 會跑以下程式碼
                    pid_t ended_child_pid = wait(&child_exit_status);
                    if (WEXITSTATUS(child_exit_status) != 7)
                    {
                        //假如不是因為找不到而結束的則會印出以下
                        printf("\nI'm parent(my_shell). Ended child pid: %d, status: %d\n", ended_child_pid, child_exit_status);
                    }
                }
                else
                {
                    // fork() 失敗
                    printf("fork() failed.\n");
                }
            }
        }
    }
    return 0;
}
