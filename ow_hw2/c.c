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

int main(){
    int running = 1;
    int shmid;
    void *shared_memory=(void *) 0;
    struct shared_use_st *shared_stuff;
    
    char buffer[BUFSIZ];
    
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
    printf("Type Ctrl+C to exit");
    shared_stuff = (struct shared_use_st *)shared_memory;
    
    while(running){
        while(shared_stuff->written_by_you ==1){
            printf("waiting for server....\n");
            sleep(1);
        }
        printf("\n-------------------------");
        printf("\nEnter some text: ");
        fgets(buffer,BUFSIZ,stdin);
        printf("waiting for server....");
        strncpy(shared_stuff->some_text,buffer,TEXT_SZ);
        shared_stuff->written_by_you =1;
        
        while (shared_stuff->written_by_you!=2){
            printf("\nwaiting for server....");
            sleep(1);
        }
        printf("\nServer Output:%s",shared_stuff->some_text);
        shared_stuff->written_by_you =0;

        if (strncmp(buffer,"end",3) == 0){
            running = 0;
        }
    }
    
    exit(EXIT_SUCCESS);
}
