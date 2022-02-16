/*
	Name - Gurpreet Singh Dhaliwal
	Id: 110039329
*/

#include "gdsmallsh.h"
#include "gdruncommand.c"
#include "gduserin.c"
#include "gdproc_line_assn4.c"
#include <string.h>


char cwd[512];
char *prompt= cwd;

int userin(char *p);
void procline(void);

int main()
{
	//to change the prompt to the current directory name 
	if(getcwd(cwd, sizeof(cwd)) == NULL){
		perror("Not able to get current directory\n");
	}
	//strcat(cwd, "$");
	while (userin(prompt) != EOF){
		procline();
		//change the prompt if the directory is updated by the cd command
		if(getcwd(cwd, sizeof(cwd)) == NULL){
			perror("Not able to get current directory\n");
		}
		printf("\n");
	}
}



