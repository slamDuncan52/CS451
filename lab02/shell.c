/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LINE		80 /* 80 chars per line, per command */

int main(void)
{
        char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
        char raw[MAX_LINE];
        int should_run = 1;

        int i, upper;

        while (should_run){   
                i = 0;
                memset(args,0, sizeof args);
                printf("mmsh-->");
                fflush(stdout);
                scanf("%s",&raw);
                printf("%s",raw);
                args[0] = strtok(raw," ");
                while(args[i] != NULL)
                {
                        i++;
                        args[i] = strtok(NULL," ");
                        //printf("%s",args[i]);
                }
                if(strcmp(args[0],"exit") == 0)
                {
                        should_run = 0;
                        continue;
                }
                /* After reading user input, the steps are:
                 * (1) fork a child process
                 */
                pid_t pid = fork();
                if(pid == 0)
                {
                        /*
                         * (2) the child process will invoke execvp()
                         */
                        execvp(args[0], args);
                } 
                else 
                {
                        /*
                         * (3) if command included &, parent will invoke wait()
                         */



                }  
        }

        return 0;
}
