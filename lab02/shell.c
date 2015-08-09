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
		char history[50];
        char background = 0;
        int fd[2];
        int should_run = 1, i, j, k, charRead;

        while (should_run) {
                i = 0;
                j = 0
				k = 0;
                charRead = 0;
                background = 0;
                fflush(stdout);
                printf("mmsh--> ");
                fgets(raw, MAX_LINE, stdin);
                raw[strlen(raw) - 1] = '\0';
                args[0] = strtok(raw, " ");

				if (k == 50) {
					for (int l = 0; l < k; l++){
						if (l+1 == k) {
							history[l] = args[0];
						} else {
							history[l] = history[l+1];
						}
					}
				}
				
				if (k < 50) {
					history[k] = args[0];
					k++;
				}
				
                while (args[i] != NULL) {
                        if (strcmp(args[i], "&") == 0) {
                                background = 1;
                                args[i] = NULL;
                        }
                        args[++i] = strtok(NULL, " ");
                }
                if (args[0] == NULL) {
                        continue;
                }
                else if (strcmp(args[0], "exit") == 0) {
                        should_run = 0;
                        continue;
                }
				else if (strcmp(args[0], "history") == 0) {
					if (int l = k-1; l > k-11; l--){
						printf("%i. %s", l, history[l]);
					}
					continue;
				} 
				else if (strcmp(args[0], "!!") == 0) {
					args[0] = history[k-1];
					continue;
				}
				else if (strstr(args[0], "!") != NULL) {
					char *res = (char*)malloc(sizeof(char)*(2));
					strncpy(res, strstr(args[0], "!")+1, 2);// Question: if the # is not 2 values will it throw a npe or aiobe?
					int index = atoi(res);
					if (index < 50 && index => 0) {
						args[0] = history[index];
					} else {
						printf("Command Not Available.");
					}
					continue;
				}
                pipe(fd);
                pid_t pid = fork();
                if (pid == 0) {
                        close(fd[1]);
                        read(fd[0], &background, sizeof(int));
                        if (background == 1) {
                                setpgid(0, 0);
                        }
                        execvp(args[0], args);
                }
                else if (pid < 0) {
                        exit(1);
                }
                else {
                        write(fd[1], &background, sizeof(int));
                        if (background == 0) {
                                wait(NULL);
                        }
                }
        }
        return 0;
}
