#include "include/jsh.h"
#include <ncurses.h>

#define ctrl(x) ((x) & 0x1f)

int main(int argc, char** argv)
{
    jshell* jsh = jsh_init();

    while (jsh->running) {
        jsh_main(jsh);
    }

    jsh_destroy(jsh);

    return 0;
}
