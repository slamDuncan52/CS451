#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* 80 chars per line, per command */

int main(void)
{
        char* args[MAX_LINE / 2 + 1]; /* command line (of 80) has max of 40 arguments */
        char raw[MAX_LINE];
        int should_run = 1;

        int i, j, charRead, noWait;

        while (should_run) 
        {
                i = 0;
                j = 0;
                charRead = 0;
                noWait = 0;
                fflush(stdout);
                printf("mmsh--> ");
                fgets(raw, MAX_LINE, stdin);
                raw[strlen(raw) - 1] = '\0';
                args[0] = strtok(raw, " ");
                while (args[i] != NULL) 
                {
                        if (strcmp(args[i], "&") == 0) 
                        {
                                noWait = 1;
                                args[i] = NULL;
                        }
                        args[++i] = strtok(NULL, " ");
                }
                if (args[0] == NULL) 
                {
                        continue;
                }
                else if (strcmp(args[0], "exit") == 0) 
                {
                        should_run = 0;
                        continue;
                }
                pid_t pid = fork();
                if (pid == 0) 
                {
                        execvp(args[0], args);
                }
                else if (pid < 0) 
                {
                        exit(1);
                }
                else 
                {
                        if (noWait == 1) 
                        {
                                continue;
                        }
                        else 
                        {
                                wait(NULL);
                        }
                }
        }
        return 0;
}
