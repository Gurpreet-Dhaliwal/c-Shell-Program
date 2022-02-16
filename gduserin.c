/*
	Name - Gurpreet Singh Dhaliwal
	Id: 110039329
*/

#include "gdsmallsh.h"
#include<signal.h>
#include <stdio.h>
#include<unistd.h>
#define TIMEOUT 10 
#define MAXTRIES 100 
#define CTRL_G '\007' 
#define TRUE 1
#define FALSE 0
static char inpbuf[MAXBUF], tokbuf[2 * MAXBUF],
*ptr = inpbuf, *tok = tokbuf;
int count =0; 

static int timed_out;

//signal handler for handling the signal 
void catch (int sig){
	timed_out = TRUE;
	puts("Hurry up! Press the Enter Key to continue...");
}

int inarg(char c);


int userin(char *p)
{
	int c, count;
	ptr = inpbuf;
	tok = tokbuf;
	count = 0;
	signal(SIGALRM, catch);
	for (;;) {
		//varaible for checking the time varaible 
		timed_out = FALSE;
		printf("Enter command in 10 seconds or you will be annoyed!\n");
		printf("%s$ ", p);
		alarm(TIMEOUT);
		fgets(inpbuf, sizeof(inpbuf), stdin);
        //printf("Buffer input : %s " , inpbuf);
		alarm(0);
		if (!timed_out)
			break;
	}
	signal(SIGALRM, SIG_DFL);
	int len = strlen(inpbuf);
	inpbuf[len] = '\0';
	//return the length of the input 
	return len;

}
int gettok(char **outptr)
{
	int type;
	
	*outptr = tok;
	
	while (*ptr == ' ' || *ptr == '\t')
		ptr++;
	
	*tok++ = *ptr;
	
	switch (*ptr++) {
	case '\n':
		type = EOL;
		break;
	case '&':
		type = AMPERSAND;
		break;
	case ';':
		type = SEMICOLON;
		break;
	case '|':
		//if the pipe symbol is there
		type = PIPE;
		break;
	case '>':
		//if redirect symbol is there 
		type = REDIRECT;
		break;
	default:
		type = ARG;
		
		while (inarg(*ptr))
			*tok++ = *ptr++;
	}
	*tok++ = '\0';
	return type;
}

//****************************************************************************************
static char special[] = { ' ', '\t', '&', ';', '\n', '\0' };
int inarg(char c)
{
	char *wrk;
	for (wrk = special; *wrk; wrk++)
	{
		if (c == *wrk)
			return (0);
	}
	return (1);
}


