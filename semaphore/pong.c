#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>

#define KEY 0x1111

enum {FAIL = -1};

struct sembuf v = {0, +1, SEM_UNDO};

int main()
{
    int id = semget(KEY, 1, 0666 | IPC_CREAT);
	
    if(FAIL == id)
    {
        perror("semget"); exit(11);
    }
  
    /* main part : prints ping whenever semaphore is 1*/
	while (1)
    {
        
		puts("pong");
       
       /*increse semaphore by 1*/ 
        if(FAIL == semop(id, &v, 1))
        {
            perror("semop p"); exit(16);
        }

        sleep(2);
    }
   
    return 0;
 }
