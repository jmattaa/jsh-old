#include "include/jsh.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    jshell_getdir();
    while (1)
    {
        printf("\n"); // print line to have space between prompts
                      // just realised that fish also has newline on top after clear command
                      // so I won't fix it (not lazy I'm working like the fish developers)


        jshell_main();
        jshell_lex();

        if (jshell_parsecmd() == 1)
            continue; // don't execute anything if there is a error
        
        // so we don't create a child process for the builtin commands
        if (jshell_builtincmd(jsh.argv[0]) == 0)
            jshell_runbuiltincmd();
        else 
        {
            if (jshell_fork() == 0)
            {
                jshell_runcmd();
                exit(0);
            }
        }
        
        wait(0); // this lets the output be prinited
                 // before the loop continues

        jshell_clean();
    }

    return 0;
}

