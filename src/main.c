#include "include/jsh.h"

#include <sys/wait.h>

int main(int argc, char **argv)
{
    while (1)
    {
        jshell_main();
        jshell_lex();
        
        if (jshell_fork() == 0)
            jshell_runcmd();

        wait(0); // this lets the output be prinited
                 // before the loop continues
    }

    return 0;
}

