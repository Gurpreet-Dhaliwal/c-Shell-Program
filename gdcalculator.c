/*
	Name - Gurpreet Singh Dhaliwal
	Id: 110039329
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/wait.h>

int main(int argc, char* argv[]) { 
    char message[]= "\nThis program makes simple arithmetics\n";
    void childfunction(char *);
    write(1, message, strlen(message));
    while (1){
        char msg[] ="\nEnter an arithmetic statement, e.g., 34 + 132\n";
        int status;
        char input[100];
        char input1[100], input2[100];
        write(1, msg, strlen(msg)); 
        read(0, input, 100);
        strcpy(input2, input);
        sscanf(input2, "%s", &input1);
        
        //for quiting the code on quit 
        if(!strcmp(input1, "quit")){
            //quit the code 
            exit(1);
        }
        if(fork() == 0){
            childfunction(input);
        }
        else{
            char msg[]  = "Created a child to make your operation, waiting\n";
            write(1, msg, strlen(msg)); 
            wait(&status);
            if(WEXITSTATUS(status) == 50){
                char msg[] ="Wrong Statement. Enter input with the proper spaces after each argument.\n";
                write(1, msg, strlen(msg)); 
            }
            else if(WEXITSTATUS(status) == 100){
                char msg[] ="Divison by Zero\n";
                write(1, msg, strlen(msg)); 
            }
            else if(WEXITSTATUS(status) == 200){
                char msg[] ="Wrong Operator\n";
                write(1, msg, strlen(msg)); 
            }
        }
        //exit(1);
    }
    return 0; 
}
void childfunction(char *line){
    char msg[] =  "I am a child working for my parent \n";
    write(1, msg, strlen(msg)); 

    int op1,op2;
    int output = 0;
	char op;
	int rt=sscanf(line, "%d %c %d %c", &op1, &op, &op2);
    
    if(rt != 3){
        exit(50);
    }
    else{
        if(op == '+'){
            output = op1 + op2;
        }
        else if(op == '-'){
            output = op1 - op2;
        }
        else if(op == '/'){
            if(op2 ==0){
                exit(100);
            }
            output = op1 / op2;
        }
        else if(op == '*'){
            output = op1 * op2;
        }
        else{
            exit(200);
        }
    }
    char out[1000];
    sprintf(out, "%d %c %d = %d \n", op1, op , op2, output);
    write(1, out, strlen(out));
    exit(0);
}
