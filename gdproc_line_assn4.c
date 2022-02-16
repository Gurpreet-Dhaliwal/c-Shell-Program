/*
	Name - Gurpreet Singh Dhaliwal
	Id: 110039329
*/

#include "gdsmallsh.h"
#include <string.h>
#include <stdlib.h>
#include<stdio.h> 
#include<unistd.h> 
#include<fcntl.h>
#include<sys/wait.h>

int gettok(char **outptr);
int runcommand(char **cline, int where);
int join(char *com1[], char *com2[]);
void redirectFun(char **command, char* filename);

void procline(void) 
{
	char *arg[MAXARG + 1]; 
	int toktype; 
	int narg = 0; 
	int type; 
	int pipe = 0, redirect =0;
	int arg1=0, arg2=0;
	int pipe_or_redirect = 0; 
	for (;;) 
	{
	   switch (toktype = gettok(&arg[narg])) {
		case ARG:
			if (narg < MAXARG) {
				narg++;
				if(pipe_or_redirect){
					arg2++;
				}
				else{
					arg1++;
				}
			}
			break;
		//for handling the pipe input 
		case PIPE:
			pipe = 1;
			pipe_or_redirect = 1;
			break;

		//for redirecting to the text file. 
		case REDIRECT:
			redirect = 1;
			pipe_or_redirect = 1;
			break;
		case EOL:
		case SEMICOLON:
		case AMPERSAND:
			//if the pipe symbol is there 
			if(pipe){
				char *command1[MAXARG + 1];
				char *command2[MAXARG + 1];
				//copy the contents in the command1 and command2 and pass it to the function. 
				int count, count2;

				//reading first part of the command before the pipe symbol
				for(count= 0; count< arg1; count++ ){
					command1[count] = arg[count];
					//printf("arg %d, %s\n", count, arg[count]);
				}
				command1[count] = NULL;

				//reading after the pipe symbol 
				for(count2= 0; count< arg1 + arg2; count2++ , count++){
					command2[count2] = arg[count];
					
				}
				command2[count] = NULL;
				join(command1, command2);
				narg = 0;	
			}

			//if the redirect variable is true 
			if(redirect){
				char *command1[MAXARG + 1];
				int count;
				//reading first part of the command before the redirect symbol
				for(count= 0; count< arg1; count++ ){
					command1[count] = arg[count];
				}
				command1[count] = NULL;
				char* filename = arg[count];
				redirectFun(command1, filename);
				narg = 0;			
			}
			if (toktype == AMPERSAND)
				type = BACKGROUND;
			else
				type = FOREGROUND;
			if (narg != 0)
			{
				arg[narg] = NULL;
				runcommand(arg, type);
			}
			if (toktype == EOL)
				return;
			narg = 0;
			break;
	  }
	}
}


