#include <stdio.h>
#include <stdlib.h> // memory management
#include <string.h> // handles strings, makes my life easier
#include <unistd.h> // ***UNIX/POSIX systems calls (fork, getpid, sleep)
#include <sys/wait.h> // waits on child processes by fork()

#define MAX_INPUT 1024
#define MAX_ARGS 64

int main(void)
{
    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    while (1)
    {
        // shell prompt
        printf("> ");
        fflush(stdout);

        // Read input
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';


        if (strlen(input) == 0)
            continue;

        // tokenization, please work
        int i = 0;
        char *token = strtok(input, " ");

        while (token != NULL && i < MAX_ARGS - 1)
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }

        args[i] = NULL;
        if (strcmp(args[0], "exit") == 0)
        {
            break;
        }

        // Built-in: cd
        if (strcmp(args[0], "cd") == 0)
        {
            if (args[1] == NULL)
            {
                fprintf(stderr, "cd: missing argument\n");
            }
            else if (chdir(args[1]) != 0)
            {
                perror("cd");
            }
            continue;
        }

        // Create child process
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork");
            continue;
        }

        if (pid == 0)
        {
            // Child executes command
            execvp(args[0], args);

            // Only reached if exec fails
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        else
        {
            // Parent waits
            waitpid(pid, NULL, 0);
        }
    }

    return 0;
}
