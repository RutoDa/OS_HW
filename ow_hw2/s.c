#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define TEXT_SZ 2048
    
struct shared_use_st{
    int written_by_you; //誰最後寫的 0是client 1是server
    char some_text[TEXT_SZ];
};

//將字串分割的function
void string_split(char *a, char *b, char *input) {
    char turn = 'a';
    int i=0, j=0;
    while(input[i]!='\n') {
        if (input[i] == ' ') {
            turn = 'b';
            a[j] = '\0';
            j = -1;
        } else if (turn == 'a') {
            a[j] = input[i];
        } else {
            b[j] = input[i];
        }
        j++;
        i++;
    }
    if (turn == 'a') {
        a[j] = '\0';
        b[0] = '\0';
    } else {
        b[j] = '\0';
    }
}

int main(){
    int shmid;
    void *shared_memory=(void *) 0;
    struct shared_use_st *shared_stuff;
    
    srand((unsigned int)getpid());
    
    shmid = shmget((key_t)5269,sizeof(struct shared_use_st),0666|IPC_CREAT);
    
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
    
    while(1){
        if (shared_stuff->written_by_you==1){
            char *input = shared_stuff->some_text;
            char a[100];
            char b[100];
            char output[101];
            int x,y,z;
            string_split(a, b, input);
            if (strlen(a)==0) x=0;
            else x = atoi(a);
            if (strlen(b)==0) y=0;
            else y = atoi(b);
            z = x+y;
            sprintf(output, "%d", z);
            
            strncpy(shared_stuff->some_text,output,TEXT_SZ);
            shared_stuff->written_by_you =2;
            sleep(rand()%4);
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