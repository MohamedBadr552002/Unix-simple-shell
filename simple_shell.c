/*
Author  : Mohamed Badr
Project : Simple shell - Multiprocessing
Data    : March 2024
*/


/******************* Includes *************************/

#include <unistd.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


/******************* Global Variables *************************/
char Input_Command[100]; //<! input command from user>
char* parsed_command[5];	//<! Array for parsing command



/*
function on_child_exit()
    reap_child_zombie()
    write_to_log_file("Child terminated")

*/





/*
function execute_command()
    child_id = fork()
    if child:
        execvp(command parsed)
        print("Error)
        exit()
    else if parent and foreground:
        waitpid(child)


*/
void execute_command(){
		int ret_id =fork();

       		 if(ret_id <0)
               		 printf("fork failed");
       		 else if(ret_id >0)
       		 {
               		// printf("this perant with id =%d \n ",getpid());
               		 int status;
			 wait(&status);
       		 }
       		 else if(ret_id == 0){
               		// printf("i am the child my id = %d \n",getpid());
			 char * newargv[] ={NULL};
			 char * newnvp[] ={NULL};
			 execve(parsed_command[0] , newargv,newnvp);
			 printf("ERROR:::: Not Found this command\n");
       		 }

}



/*
function execute_shell_bultin()
    swirch(command_type):
        case cd:
        case echo:
        case export:

*/

void execute_shell_bultin()
{

	if(!strcmp(parsed_command[0],"cd"))
	{
		
		printf("%s \n",parsed_command[1]);
		chdir(parsed_command[1]);
	}
	else if (!strcmp(parsed_command[0],"echo"))
	{
	
	}
	else if (!strcmp(parsed_command[0],"export"))
	{
	
	}
	else
		printf("Command Faild !!\n");

}






/*
			funcation description
*/

char** parse_input(){
	
	char* token ;
	
	fgets(Input_Command,100, stdin);
	
	if((strlen(Input_Command)>0) && (Input_Command[strlen(Input_Command)-1] == '\n'))
		Input_Command[strlen(Input_Command)-1] ='\0';	
		
	
	token = strtok(Input_Command, " ");
	
	int i=0;
	while(token != NULL){
	
		parsed_command[i] = token;
		i++;
		token = strtok(NULL, " ");
	}
	
	return parsed_command;
	
}


/*
function shell()
    do
        parse_input(read_input())
        evaluate_expression():
        switch(input_type):
            case shell_builtin:
                execute_shell_bultin();
            case executable_or_error:
                execute_command():

    while command_is_not_exit

*/
void shell()
{	

	
	do
	{	
		//parse input 
		printf("ready $> ");
		if(parse_input() == NULL){
			printf("Parsing Fail !! \n");
			continue;
		}
			
		
		if(strlen(Input_Command) == 0)
			continue;
			
		//todo ----> evaluate_expression
		
		
		// Command Switching
		if((!strcmp(parsed_command[0],"cd")) || (!strcmp(parsed_command[0],"echo")) || (!strcmp(parsed_command[0],"export")))
		{
			execute_shell_bultin();
		}else{
		
			printf("this command should executed \n");
			execute_command();
		}
		
		
	/*		
	printf("%s \n" , Input_Command);
	
	for(int i=0;i<5;i++){
	
		printf("%s \n",parsed_command[i]);
	}
	*/
	}
	while(strcmp(parsed_command[0],"exit")); //<---------


}



/*
function setup_environment()
    cd(Current_Working_Directory)

*/
void setup_environment()
{
	if(chdir("/usr/bin/")) // On  success,  zero is returned.
	{
		printf("No such file or Directory/n");
	}

}


/*
function parent_main()
    register_child_signal(on_child_exit())
    setup_environment() 
    shell()
*/


int main()
{
	//todo -> regiter child signla
	
	
	
	//Setup environment
	setup_environment();
	
	// Jump to shell program 
	shell();
	
	//
	printf("program ended\n");

	
	
	return 0;	
	
}

