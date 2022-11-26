#include <stdlib.h> 
#include <unistd.h> /* needed to define exit() */
#include <errno.h> /* needed for fork() and getpid() */ /* needed for errno */
#include <stdio.h> /* needed for printf() */
#include <string.h>

void find_path(char *input, char *path);
void clear (void);

int main () {
    char bg = 0;
    char have_child = 0;
    pid_t parent_pid = getpid();
    pid_t pid;
    printf("Parent pid is %d\n", parent_pid);
    while (1) {
        char input[50]; //存 input 的字串
        char path[50];
        int child_exit_status;
        printf("1094841 ms> "); // print 出提示符號
        
        if (fgets(input, sizeof(input), stdin)) 
        {
            bg = 0;
            
            if (input[strlen(input)-2]=='&') bg = 1;

            find_path(input, path);
        
            if (bg == 1) {
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
                    exit(1);
                } else {
                    pid_t ch = wait(&child_exit_status);
                    //printf("\nI'm parent. waitpid %d, status %d\n", pid, child_exit_status);
                }
            }   
        }
    }
    return 0;
}

void find_path(char *input, char *path) {
    for (int i = 0; i < strlen(input); i++) {
        if (input[i] == ' ') {
            path[i] = '\0';
            break;
        } else if (input[i] == '\n') {
            path[i] = '\0';
        } else {
            path[i] = input[i];
        }
    }
}

