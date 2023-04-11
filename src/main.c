#include "include/jsh.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    while (1)
    {
        jshell_main();
        jshell_lex();
        
        if (jshell_fork() == 0)
        {
            jshell_runcmd();
            exit(0);
        }

        wait(0); // this lets the output be prinited
                 // before the loop continues

        printf("\n"); // print line to have space between prompts
                      // just realised that fish also has newline after clear command
                      // so I won't fix it (not lazy)

        jshell_clean();
    }

    return 0;
}

