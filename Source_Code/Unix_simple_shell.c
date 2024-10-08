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
#include <stdlib.h>
#include <signal.h>

typedef struct {
	char name[30];
	char value[30];
}Data;


/******************* Global Variables *************************/
char Input_Command[256]; 		//<! input command from user>
char* parsed_command[16];		//<! Array for parsing command
char systemBuff[] = "/usr/bin/";	//Absolute path
int i=0;				// iteration variable	
int ret_id;
Data data_storage[10];			//array for saving data from export command
int data_counter;
int k;


/******************* Function Decleration *************************/
void setup_environment();
void execute_command();
void execute_shell_bultin();
void parse_input(char* split);
void shell();
void on_child_exit(int signl_nu);




/*
function on_child_exit()
    reap_child_zombie()
    write_to_log_file("Child terminated")

*/

void on_child_exit(int signl_nu)
{
	int status;
	pid_t pid;
	
	
	while((pid = waitpid(-1, &status,WNOHANG)) >0) ;
	
	
	//printf("[LOG] child proccess terminated.\n");
	FILE *pFile;
        pFile = fopen("log.txt", "a");
        if(pFile==NULL) perror("Error opening file.");
        else fprintf(pFile, "[%d] child proccess terminated.\n",pid);
        fclose(pFile);
	

}




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
		 ret_id =fork();

       		 if(ret_id == -1)
               		 printf("fork failed");
       		 else if(ret_id == 0){

			 //printf("%s \n",parsed_command[0]);
			 execvp(parsed_command[0] , parsed_command);
			 printf("ERROR :::: Not Found this command\n");
        		// exit(EXIT_SUCCESS);
       		 }
		 else 
       		 {
               		if(parsed_command[i] == NULL)
			 	waitpid(ret_id , NULL,0);
			 	
			 	
			// Reset the parsed_command array
        		for (int j = 0; j < 16; j++) {
            			parsed_command[j] = NULL;
        		}
       			i = 0; // Reset the iteration variable	
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
		if(!strcmp(parsed_command[1],"~"))
		{
			setup_environment();
			printf("/home/badr/ \n");
		}
		else{
		printf("%s \n",parsed_command[1]);
		chdir(parsed_command[1]);
		}
	}
	else if (!strcmp(parsed_command[0],"echo"))
	{
		 int i = 1;
        	// print all arguments to the right of the "echo" command
        	//strcmp(parsed_command[i],""
        	while(parsed_command[i]){
            		printf("%s", parsed_command[i]);
            		printf("\t");
            		i++;
        	}
        	printf("\n");
	}
	else if (!strcmp(parsed_command[0],"export"))
	{	
		strcpy(Input_Command , parsed_command[1]);
		parse_input("=");
		
		//Save data in data array storage
		strcpy(data_storage[data_counter].name, parsed_command[0]);
		strcpy(data_storage[data_counter].value ,parsed_command[1]);
		data_counter++ ;
		
		/*
		for(int k=0;k<5;k++)
		{
			printf("%s \t",data_storage[k].name);
			printf("%s \n",data_storage[k].value);
		}*/
		
	}
	else
		printf("Command Faild !!\n");
		
				 	
	// Reset the parsed_command array
        for (int j = 0; j < 16; j++) {
            parsed_command[j] = NULL;
        }
       	i = 0; // Reset the iteration variable		
	

}






/*
			funcation description
*/





void parse_input(char* split){
	
	
		//parse input 
		char* token ;
		token = strtok(Input_Command, split);
	
		i=0;
		while(token != NULL){
	
			parsed_command[i] = token;
			i++;
			token = strtok(NULL, split);
		}
	
		
		//Check & command
		if(!strcmp(parsed_command[i-1],"&")){
			parsed_command[i-1] = NULL;
			parsed_command[i] = "&";
			
		}
		else
		{
			parsed_command[i] =NULL;
		}
		/*	
		for(int i=0;i<5;i++){
	
		printf("%s \n",parsed_command[i]);
		}*/
			
		
		// evaluate_expression
		
		
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

	
	while(1)
	{	
		
		
		printf("ready $: ");
		fgets(Input_Command,100, stdin);
	
	
		if((strlen(Input_Command)>0) && (Input_Command[strlen(Input_Command)-1] == '\n'))
			Input_Command[strlen(Input_Command)-1] ='\0';	
			
			
		if(!strcmp(Input_Command,""))
			continue;
			
		if(!strcmp(Input_Command,"exit"))
			break;		
		
		parse_input(" ");
		
		//evaluate expretion 
		
		k=1;
        	while (parsed_command[k])
        	{	
            	// Check if the argument starts with '$'
            	if (parsed_command[k][0] == '$')
            	{	
                	// Find the variable in data_storage
                	for (int j = 0; j < data_counter; j++)
                	{	
                    		if (strcmp(parsed_command[k] + 1, data_storage[j].name) == 0)
                    		{	
                    			strcpy(parsed_command[k],data_storage[j].value);
                        		//printf("%s ", data_storage[j].value);
                        		break;
                    		}
                	}
            	}

            		k++;
        	}
		
		
		/*
		for(int c=0;c<5;c++){
	
			printf("%s \n",parsed_command[c]);
		}*/
		
		// Command Switching
		if((!strcmp(parsed_command[0],"cd")) || (!strcmp(parsed_command[0],"echo")) || (!strcmp(parsed_command[0],"export")))
		{
			execute_shell_bultin();
		}else{
			

			execute_command();
		}
		
		
	/*		
	printf("%s \n" , Input_Command);
	
	for(int i=0;i<5;i++){
	
		printf("%s \n",parsed_command[i]);
	}
	*/
	}
	

}



/*
function setup_environment()
    cd(Current_Working_Directory)

*/
void setup_environment()
{
	if(chdir("/home/badr/")) // On  success,  zero is returned.
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

	
	// tie the handler to the SGNCHLD signal
	signal(SIGCHLD, on_child_exit);
	
	//Setup environment
	setup_environment();
	
	// Jump to shell program 
	shell();
	
	//
	printf("program ended\n");

	
	
	return 0;	
	
}
