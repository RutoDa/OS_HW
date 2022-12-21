#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define TEXT_SZ 2048
    
struct shared_use_st{
    int written_by_you;
    char some_text[TEXT_SZ];
};
void string_split(char *a, char *b, char *input);

int main(){
    int running = 1;
    int shmid;
    void *shared_memory=(void *) 0;
    struct shared_use_st *shared_stuff;
    
    srand((unsigned int)getpid());
    
    shmid = shmget((key_t)1234,sizeof(struct shared_use_st),0666|IPC_CREAT);
    
    if (shmid == -1){
        fprintf(stderr,"shmget failed\n");
        exit(EXIT_FAILURE);
    }
    
    shared_memory = shmat(shmid,(void *) 0,0);
    
    if (shared_memory == (void *)-1){
        fprintf(stderr,"shmat failed\n");
        exit(EXIT_FAILURE);
    }
    
    printf("pid:%d\n", getpid());
    printf("Memory attached at %X\n",(int)shared_memory);
    printf("Type Ctrl+C to exit\n");
    shared_stuff = (struct shared_use_st *)shared_memory;
    shared_stuff->written_by_you = 0;
    
    while(running){
        if (shared_stuff->written_by_you){
            // char a[20];
            // char b[20];
            // if (strlen(shared_stuff->some_text)!=0)
            //     string_split(a, b, shared_stuff->some_text);
            printf("You wrote:%s",shared_stuff->some_text);
            sleep(rand()%4);
            shared_stuff->written_by_you = 0;
            
            if (strncmp(shared_stuff ->some_text,"end",3) == 0){
                running =0;
            }
        }
    }
    
    if (shmdt(shared_memory)==-1){
        fprintf(stderr,"shmdt failed\n");
        exit(EXIT_FAILURE);
    }
    if (shmctl(shmid,IPC_RMID,0) == -1){
        fprintf(stderr,"shmctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
        
    exit(EXIT_SUCCESS);
}

void string_split(char *a, char *b, char *input) {
    char turn = 'a';
    int i;
    for (i = 0; i < strlen(input); i++) {
        if (input[i] == ' ') {
            turn = 'b';
            a[i] = '\0';
        } else if (turn = 'a') {
            a[i] = input[i];
        } else {
            b[i] = input[i];
        }
    }
    b[i] = '\0';
}