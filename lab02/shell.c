/***********************************/
/* Mitchell Duncan and Mikayla Ray */
/* CS451 Summer 2015               */
/* Lab 02                          */
/***********************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* 80 chars per line, per command */

int main(void)
{
        char* args[MAX_LINE / 2 + 1]; /* command line (of 80) has max of 40 arguments */
        char raw[MAX_LINE];/* Unparsed command line */
        char history[50][MAX_LINE]; /* Command history */
        char background = 0; /* & flag */
        int fd[2]; /* For piping background to child */
        int should_run = 1, bgFind, charRead;
        int hisLen = 0;/* Length of history */
        printf("mmsh--> ");//Print prompt

        while (should_run) {
                bgFind = 0;//Reset vars
                charRead = 0;
                background = 0;
                fflush(stdout);
                fgets(raw, MAX_LINE, stdin);//Get next user input
                raw[strlen(raw) - 1] = '\0';//Assure null terminated
                args[0] = strtok(raw, " ");//Grab command as first word

                if (args[0] == NULL) {//If no command
                        continue;
                }
                else if (strcmp(args[0], "exit") == 0) {//If exit command
                        exit(0);
                }

                if (hisLen == 50) {//If history full
                        //Loop through and shuffle history down
                        for (int l = 0; l < hisLen; l++) {
                                if (l + 1 == hisLen) {
                                        strcpy(history[l], args[0]);
                                }
                                else {
                                        strcpy(history[l], history[l + 1]);
                                }
                        }
                }

                if (hisLen < 50) {//Otherwise, add to history
                        strcpy(history[hisLen], args[0]);
                }

                if (strcmp(args[0], "history") == 0) {//If command is history
                        int l = 0;
                        for (l = hisLen; l >= 0; l--) {//Print last 10 commands
                                if (history[l] != NULL) {
                                        printf("%i. %s\n", l, history[l]);
                                }
                                continue;
                        }
                }
                else if (strcmp(args[0], "!!") == 0) {//If command is !!
                        if (hisLen == 0) {//Sanity check
                                printf("No Command In History\n");
                                continue;
                        }
                        *args = history[hisLen - 1];//Set to last command
                }
                else if (strstr(args[0], "!") != NULL) { //Execute the nth command in history
                        char* res = (char*)malloc(sizeof(char) * (2));//Grab the given int
                        strncpy(res, strstr(args[0], "!") + 1, 2);
                        int index = atoi(res);
                        if (index < 50 && index >= 0) {//Sanity check
                                if (history[index] == NULL) {
                                        printf("No Command In History\n");
                                        continue;
                                }
                                else {
                                        args[0] = history[index];//Set to last command
                                }
                        }
                        else {
                                printf("No Command In History\n");
                                continue;
                        }
                }

                while (args[bgFind] != NULL) {//Search for & at end and tokenize input
                        if (strcmp(args[bgFind], "&") == 0) {
                                background = 1;//Set flag
                                args[bgFind] = NULL;//Remove & char
                        }
                        args[++bgFind] = strtok(NULL, " ");//Tokenize
                }

                pipe(fd);//Preare pipe
                pid_t pid = fork();
                if (pid == 0) {//Child
                        close(fd[1]);
                        read(fd[0], &background, sizeof(int));//Get background flag
                        if (background == 1) {
                                setpgid(0, 0);//Set background process
                        }
                        execvp(args[0], args);//Execute
                        break;//Something went wrong
                }
                else if (pid < 0) {//Something went very wrong
                        exit(1);
                }
                else {//Parent
                        write(fd[1], &background, sizeof(int));//Send bg flag
                        hisLen++;//Increment history length
                        if (background == 0) {//Wait if foreground
                                wait(NULL);
                        }
                        printf("mmsh--> ");//Print new prompt
                }
        }
        return 0;
}
