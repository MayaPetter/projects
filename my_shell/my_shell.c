/******************************************
* Author:   Maya Petter                   *
* Reviewer: Omri                          *
* Purpose:  Aprroved                      *
*******************************************/
#include <stdlib.h>    /* abort   */
#include <stdio.h>     /* puts    */
#include <string.h>    /* strtod  */
#include <sys/types.h> /* pid_t   */
#include <unistd.h>    /* fork    */  

#define COMMAND_LINE printf("\033[1;31m%s >> \033[0m", getenv("USER"))

int CheckForExit(char *user_input);
void Parser(char *program, char **arg_list);
int Spawn(char *program, char **arg_list);

int main()  
{   
    int exit = 0;
    char user_input[100] = "";    
    char *arg_list[100] = {NULL};
    
	puts("***** My Shell - for exiting write exit ************\n");
	
	while(!exit)
	{
		COMMAND_LINE;
		
		fgets(user_input, sizeof(user_input), stdin);	

		exit = CheckForExit(user_input);
		
		Parser(user_input, arg_list);
		
		Spawn(*arg_list, arg_list);
	}    

    return 0;  
} 

/***** check if user input is exit ********************************************/
int CheckForExit(char *user_input)
{
	return (0 == strcmp(user_input, "exit\n"));
}

/***** parsing the string into tokens *****************************************/
void Parser(char *program, char **arg_list)
{
	size_t index = 0;

	arg_list[index] = strtok(program, " \n");

	while (NULL != arg_list[index])	
	{
		++index;
		arg_list[index] = strtok(NULL, " \n");
	}
}

/***** open new child proccess and execute it *********************************/
int Spawn(char *program, char **arg_list)  
{   
    pid_t child_pid = fork();        

    if (child_pid != 0)
    {
        return child_pid;    
    }            
    else if (0 > child_pid)
    {
        fprintf (stderr,"an error occurred in fork\n");
        return 1;
    }
    else
    { 
        execvp (program, arg_list);            
        fprintf (stderr, "an error occurred in execvp\n");      
        abort();    
    }
    
    return 0;  
}  

