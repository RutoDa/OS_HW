#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PERMS 0666
#define SHMKEY ((key_t)1234)
#define MAXBUF 2048

int inputchat(char *username)
{
  int shmid, str_len;
  key_t key;
  char *shm;
  char str_buf[MAXBUF] = "";
  char str2_buf[MAXBUF] = "";
  char speaker[100];
  printf("p1 \n");
  if ((shmid = shmget(SHMKEY, MAXBUF + 1, PERMS)) < 0)
  {
    perror("shmget");
    exit(1);
  }
  printf("p3 \n");
  if ((shm = shmat(shmid, NULL, 0)) == (char *)-1)
  {
    perror("shmat");
    exit(1);
  }
  printf("p5 \n");
  strcpy(speaker, username);
  strcat(speaker, ":");
  printf("Hello %s, Enter Words: n", username);
  printf("p7 \n");
  while (1)
  {
    printf("type something: ");
    strcpy(str_buf, "");
    fgets(str_buf, 2000, stdin);
    //    strcat(str2_buf, str_buf);
    str_len = strlen(str2_buf);
    strcpy(shm, str_buf);
    //    strcpy(shm, "something");
    sleep(1);
  }
}

/*
int outputchat(){
  int shmid;
  key_t key;
  char *shm, *s;
  if ((shmid = shmget(SHMKEY, MAXBUF+1, IPC_CREAT | PERMS)) < 0) {
    perror("shmget");
    exit(1);
  }
  if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
    perror("shmat");
    exit(1);
  }
  while(1){
    shm[MAXBUF] = 1;
    while(strstr(shm, "n") == NULL){
      if(shm[MAXBUF] == 1){
        continue;
      }
    }
    printf("%s ", shm);
    sleep(1);
    strcpy(shm, "");
  }
}
*/

int main(int argc, char **argv)
{
  pid_t fpid;
  char c;

  printf("pid %d\n", getpid());

  inputchat("sh_m");
  return 0;
}
