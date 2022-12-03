#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int count(char* buffer)
{
    int count=0;
    //do some stuff here, come on this should be easy guys!
    return count;
}
 
int main(int argc, char **argv)
{
  int a, b, i, j;
  char c;
  char answer[5], guess[256];

  printf("my pid is %d\n", getpid());
  printf("(%s) Guess a 4-digit number. Type \'q\' to quit.\n", argv[0]);
  while (1) {
    srand( (unsigned) time(0));
    while (1) {
	answer[0] = '0' + (rand()%5);
	answer[1] = '0' + (rand()%5);
	answer[2] = '0' + (rand()%5);
	answer[3] = '0' + (rand()%5);
	if (answer[0] != answer[1] && answer[0] != answer[2] && answer[0] != answer[3] && answer[1] != answer[2] && answer[1] != answer[3] && answer[2] != answer[3]) break;
    } // make answer
    answer[4] = '\0';
    //    printf(" ans %s\n", answer);
    while(1)
        {
                //print the prompt
	  printf("(%s) Guess: ", argv[0]);
                //get input
                fscanf(stdin, "%s", guess);
		guess[4] = '\0';
		//		printf(" input %s\n", guess);
		if (guess[0] == 10) continue;
		if (guess[0] == 'q') break;
		if (guess[0] < '0' || guess[0] > '9' || guess[1] < '0' || guess[1] > '9' || guess[2] < '0' || guess[2] > '9' || guess[3] < '0' || guess[3] > '9')
		  {
		    printf("invalid number. try again\n");
		    continue;
		  }
		a = b =0;
		for (i = 0; i < 4; i++) {
		  if (answer[i] == guess[i]) a++;
		  for (j = 0; j < 4; j++) {
		    if (i == j) continue;
		    if (guess[i] == answer[j]) {
		      b++;
		      break;
		    }
		  }
		}
		printf(" %d A, %d B\n", a, b);
		if (a == 4) {
		  printf("Bingo. Continue (Y/N)? ");
		  break; // leave guessing for this term
		}
		//		  if (c == 'Y')
		//}
        } // end this term
    if (guess[0] == 'q') break;
    while ( (c = fgetc(stdin)) != 'Y' && c != 'y' && c != 'N' && c!= 'n') ;
    if (c == 'Y' || c == 'y') continue;
    else break;
  }
  return 0;
}