#include "include/jsh.h"

#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

struct jshell jsh = {
    .defaultprompt = NULL,
    .line = NULL,
    .linelen = 0,
    .argv = NULL,
    .argc = 0,
    .builtincmd = {"exit", "clear", "cd"},
}; 

void jshell_getdir()
{
    getcwd(jsh.cwd, sizeof(jsh.cwd));
}

void jshell_main()
{
    char *line = NULL;
    size_t linelen = 0;

    size_t defaultPromptLen;
    jsh.defaultprompt = jsh.cwd;

    printf("%s\n>", jsh.defaultprompt);
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
        exit(0);
    } 
    else if (strcmp(cmd, "clear") == 0)
    {
        printf("\033[2J\033[1;H");
        return;
    }
    else if (strcmp(cmd, "cd") == 0)
    {
        char *dir;

        if (jsh.argc > 2)
            fprintf(stderr, "cd: too many arguments");
        if (jsh.argc == 1) // cd with no arguments cd into user home
            dir = getenv("HOME");
        else if (jsh.argc == 2)
            dir = jsh.argv[1];

        if (chdir(dir) != 0)
            perror("cd");

        jshell_getdir();

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

