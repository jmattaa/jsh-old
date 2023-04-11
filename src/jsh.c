#include "include/jsh.h"

#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

struct jshell jsh = {
    .defaultprompt = "dir\n>",
    .line = NULL,
    .linelen = 0,
    .argv = NULL,
    .argc = 0,
    .builtincmd = {"exit", "clear"},
}; 

void jshell_main()
{
    char *line = NULL;
    size_t linelen = 0;

    printf("%s", jsh.defaultprompt);
    ssize_t charsread = getline(&line, &linelen, stdin);

    if (charsread == -1)
        exit(-1);

    jsh.linelen = linelen;
    jsh.line = calloc(linelen, sizeof(char *));
    strcpy(jsh.line, line);

    // replace newline with null terminator
    // to have jsh.line in the right format 
    size_t newline_idx = strcspn(jsh.line, "\n");
    if (newline_idx < jsh.linelen) 
    {
        jsh.line[newline_idx] = '\0';
    }

    free(line);
}

void jshell_error(char *err, int callperror)
{
    if (callperror)
        perror(err);
    else
        fprintf(stderr, "jsh: %s", err);
    exit(1);
}

void jshell_lex()
{
    if (strcmp(jsh.line, "") == 0) return;

    // split string into command and arguments
    char *token = strtok(jsh.line, " ");

    // it's never null
    // if we've passed a filename for the command
    if (token != NULL)
    {
        jsh.argc = 0;
        jsh.argv = malloc((jsh.argc+1) * sizeof(char *));
    }

    while (token != NULL)
    {
        if (strlen(token) > 0) 
        {  
            jsh.argc++;
            jsh.argv = realloc(jsh.argv, (jsh.argc+1) * sizeof(char *));
            jsh.argv[jsh.argc-1] = strdup(token);
        }

        token = strtok(NULL, " ");
    }
}

char *jshell_getfullpath(char *cmd)
{
    char *path, *path_copy, *path_token, *file_path;
    int command_length, directory_length;
    struct stat buffer;

    path = getenv("PATH");

    if (path)
    {
        path_copy = strdup(path);
        command_length = strlen(cmd);

        path_token = strtok(path_copy, ":");

        while(path_token != NULL)
        {
            directory_length = strlen(path_token);
            file_path = malloc(command_length + directory_length + 2); 

            strcpy(file_path, path_token);
            strcat(file_path, "/");
            strcat(file_path, cmd);
            strcat(file_path, "\0");

            if (stat(file_path, &buffer) == 0)
            {
                free(path_copy);

                return (file_path);
            }
            else 
            {
                free(file_path);
                path_token = strtok(NULL, ":");

            }

        }
        free(path_copy);

        if (stat(cmd, &buffer) == 0)
        {
            return (cmd);
        }


        return (NULL);

    }


    return NULL; 
}

void jshell_parsecmd()
{

}

// wraps around fork funktion
int jshell_fork()
{
    int pid;
    pid = fork();
    if (pid == -1)
        jshell_error("jsh: fork", 1);

    return pid;
}

int jshell_builtincmd(char *cmd)
{
    for (int i = 0; i < JSH_BUILTINCMD_COUNT; i++)
    {
        if (strcmp(jsh.builtincmd[i], cmd) == 0)
            return 0;
    }

    return 1;
}

void jshell_runbuiltincmd()
{
    char *cmd = jsh.argv[0];
    if (strcmp(cmd, "exit") == 0)
    {
        kill(getppid(), SIGTERM); // kill parent pid process
        exit(0);
    } 
    else if (strcmp(cmd, "clear") == 0)
    {
        printf("\033[2J\033[1;H");
        return;
    }

    // this should never be reached if all builtin commands are taken care of 
    // in the if statement above
    jshell_error("builtin command not found\njsh: unexpected error", 0);
}

void jshell_runcmd()
{
    if (jsh.argv[0] == 0)
        jshell_error("no filename for command!\n", 0);

    if (jshell_builtincmd(jsh.argv[0]) == 0)
    {
        jshell_runbuiltincmd();
        return; // if it was a builtin command 
                // then don't run execvp
    }

    if (execvp(jsh.argv[0], jsh.argv) == -1)
        perror("jsh");
}

void jshell_clean()
{
    free(jsh.line);
    free(jsh.argv);

    jsh.argv = NULL;

    jsh.argc = 0;
    jsh.linelen = 0;
}

