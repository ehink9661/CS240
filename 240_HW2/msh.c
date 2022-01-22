/* msh.c
 *
 * CS 240-01.Wilder..........Ethan Hinkle
 * 2 October, 2021........hink9661@vandals.uidaho.edu
 *
 * This is homework 2, creating a basic unix shell using fork and exec
 * This assignment required research into how fork and exec work
 *--------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* needed for tokenizer */
#include <unistd.h>
#include <sys/types.h> /* needed for fork */
#include <unistd.h> /* needed for fork */

void input(char *s);
int tokenize(char *s, char *cmd[]);
void bin(char *cmd[]);

int main(){
    char s[255]; /* biggest input 255 characters */
    char *cmd[255]; /* stores the commands individually */
    while(1){ /* loop to never leave shell */
        printf(">:");
        input(s); /* fetch input */
        if(tokenize(s, cmd)){ /* tokenize s */
            if(!(strcmp(cmd[0], "exit"))){ /* if the first command is exit */
                return 0; /* returns 0, breaking loop and ending program */
            }
            bin(cmd);
        }
    }
}

void input(char *s){ /* gets input on command line */
    int i = 0; /* needs reset every loop */
    while ((s[i] = getchar()) != '\n'){ /* \n instead of \0 for the end of the command */
        i++;
    }
    //printf("%s", s); /* debugging check */
    s[i] = '\0'; /* replaces \n at end of string with \0 for tokenizing */
}

int tokenize(char *s, char *cmd[]){ /* creates array of tokens for execvp */
    char *token;
    int j = 0;
    token = strtok(s, " "); /* get first token, up to space */
    if(token == NULL){ /* no commands entered, fixed edge case with ' ' and '\n' */
        return 0;
    }
    while (token != NULL){ /* check to see if there are any tokens left in s */
        cmd[j] = token;
        j++; /* increments cmd to store next command token */
        token = strtok(NULL, " "); /* get next token */
    }
    cmd[j] = NULL; /* execvp needs an array ending in NULL to work */
    return 1; /* have tokenized commands */
}

void bin(char *cmd[]){ /* creates path and forks */
    char path[255]; /* full path of command */
    pid_t p; /* stores value of fork() */
    strcpy(path, "/bin/"); /* sets the beginning of the path to /bin/ */
    strcat(path, cmd[0]); /* adds the command to the end of path_name */
    //printf("%s", path); /* debugging only */
    if(!(access(path, F_OK))){ /* does the command exist? returns 0 when yes */
        p = fork();
        if(p == -1){ /* fork() had an error */
            printf("An error occurred while calling fork()\n");
        }
        else if(p == 0){ /* child fork */
            execvp(path, cmd);
            //printf("Child\n"); /* debugging */
        }
        else{ /* parent fork */
            wait(NULL); /* delays return of >: so command outputs don't print on it */
            //printf("Parent\n"); /* debugging */
        }
    }
    else{
        printf("This is an invalid command.\n");
    }
}