/*
	Name - Gurpreet Singh Dhaliwal
	Id: 110039329
*/

#include "gdsmallsh.h"
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include<fcntl.h>
int runcommand(char **cline, int type)
{
	pid_t pid;
	int status;
	int i;
	switch (pid = fork()) {
		case -1:
			perror("smallsh");
			return (-1);
		case 0:
			//for terminating the program on quit or when exit is pressed
			//check if the string matches the  quit or exit command
			if(!(strcmp(cline[0], "quit")) || (!strcmp(cline[0], "exit"))){
				kill(getppid(), SIGINT);
				exit(1);
			}
			//to execute the cd command to change the directory
			else if(!strcmp(cline[0], "cd")){
				//this is the child process it will ask the parent to change the directory
				//as the changes made by the child in the directory will not be reflected in the parent side. 
				exit(10);
			}
			else if(!strcmp(cline[0], "calculator")){
				//execute the calculator file. 
				execl("./gdcalculator", "gdcalculator", NULL);
			}
			else{
			execvp(cline[0], cline);
			perror(*cline);
			exit(1);
			}
	}
	//code for parents
	if (type == BACKGROUND)
	{
		printf("[process id %d]\n", pid);
		return (0);
	}
	if (waitpid(pid, &status, 0) == -1){
		return (-1);

	}
	else{
		int exitvalue = WEXITSTATUS(status);
		//check the status value if the value 
		if(exitvalue == 10){
			//change the directory 
			//check if the directory name is given to change 
			int c;
			if(cline[1] == NULL){
				char curdir[256];
				char homepath[256] = "/";
				getcwd(curdir, sizeof(curdir));
				//to extract the home path form the current pos. 
				char* ptr = strtok(curdir, "/");
				int count= 0;
				while(ptr != NULL)
				{
					strcat(homepath, ptr);
					strcat(homepath, "/");
					ptr = strtok(NULL, "/");
					if(++count >=2){
						break;
					}
				}
				c = chdir(homepath);
				if(c < 0){
					printf("No such Directory\n"); 
				}
			}
			else if(cline[2] != NULL){
				printf("To many arguments for the cd.. please try with correct format\n");
				return(status);
			}
			else{
				c = chdir(cline[1]);
				if(c < 0){
					printf("No such Directory\n"); 
				}
				else{ //printf("changed\n");
				}
			}		
		}
		return (status);
	}
}

//for redirecting the output to the text file.
void redirectFun(char **command, char* filename)
{
	int fd;
	//open the file 
	if((fd=open(filename ,O_CREAT | O_RDWR | O_TRUNC, 0777)) == -1){
		perror("unable to create the file: ");       
	}	
	//create child process to write 
	if (!fork()) {
		close(1);//Close stdout
		dup(fd);
		execvp(command[0], command);
	}
	//parent porcess; 
	else {
		//close the file discriptor 
		close(fd);
		wait(NULL);
	}
}


//the join process 
int join(char *com1[], char *com2[]) {
	int p[2], status;
	switch (fork()) {
		case -1: 
			perror("1st fork call in join\n");
			exit(3);
		case 0: 
			break;
		default: 
			wait(&status);
			return(status);
	}
	
	//child process's code
	if (pipe(p) == -1){
		perror("pipecall in join \n");
		exit(4);
	}

	switch (fork()) {
		case -1:
			perror("2nd fork call failed in join\n");
			exit(5);
		case 0:  //the writing process (grandchild procss)
			dup2(p[1], 1);  //redirect stdout to the pipe, writing end.	
			close(p[0]);
			close(p[1]);
			execvp(com1[0], com1);
			perror("1st execvp call failed in join\n");
			exit(6);
		default: //the "parent" process's code
			dup2(p[0], 0); //redirect stdin to the pipe, reading end

			close(p[0]);
			close(p[1]);

			execvp(com2[0], com2);

			perror("2nd execvp call failed in join\n");
			exit(7);
		}
}

