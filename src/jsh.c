#include "include/jsh.h"

jshell* jsh_init()
{
    jshell* jsh = calloc(1, sizeof(jshell));

    jsh->current_command_s = 0;
    jsh->running = 1;

    initscr();
    raw();
    noecho();

    return jsh;
}

void jsh_main(jshell* jsh)
{
    printw("[jsh] > %s", jsh->current_command);

    int status;

    jsh->ch = getch();
    switch (jsh->ch) {
    case jsh_ctrl('q'):
        jsh->running = 0;
        break;
    case KEY_BACKSPACE:
    case KEY_DC:
    case 127:
        if (jsh->current_command_s != 0)
            jsh->current_command[(jsh->current_command_s--) - 1] = 0;
        break;
    case KEY_ENTER:
        char *res = brain_execute(jsh->current_command, status);
        break;
    default:
        jsh->current_command[jsh->current_command_s++] = jsh->ch;
        break;
    }
    erase();
}

void jsh_destroy(jshell* jsh)
{
    refresh();
    endwin();

    if (jsh) free(jsh);
}
