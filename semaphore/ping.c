#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define KEY 0x1111

enum {FAIL = -1};

struct sembuf p = {0, -1, SEM_UNDO};

int main()
{

    int id = semget(KEY, 1, 0666 | IPC_CREAT);
	
    if(FAIL == semctl(id, 0, SETVAL, 0))
    {
        perror("semctl"); exit(12);
    }
    
   while (1)
    {
      	puts("ping");
       
        /*decrese semaphore by 1*/ 
        if(FAIL == semop(id, &p, 1))
        {
            perror("semop p"); exit(16);
        }
    }
    
    return 0;
 }
