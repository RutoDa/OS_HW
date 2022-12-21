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
    int shmid;
    void *shared_memory=(void *) 0;
    struct shared_use_st *shared_stuff;
    
    char buffer[BUFSIZ];
    
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
    printf("Type q to exit");
    shared_stuff = (struct shared_use_st *)shared_memory;
    
    while(1){
        while(shared_stuff->written_by_you ==1){
            printf("waiting for server....\n");
            sleep(1);
        }
        printf("\n-------------------------");
        printf("\nEnter two integers(or type q to exit): ");
        fgets(buffer,BUFSIZ,stdin);
        if (strcmp(buffer,"q\n") == 0){
            break;
        }

        printf("waiting for server....");
        strncpy(shared_stuff->some_text,buffer,TEXT_SZ);
        shared_stuff->written_by_you =1;
        
        

        while (shared_stuff->written_by_you!=2){
            printf("\nwaiting for server....");
            sleep(1);
        }
        printf("\nServer Output:%s",shared_stuff->some_text);
        shared_stuff->written_by_you =0;

    }
    
    exit(EXIT_SUCCESS);
}
