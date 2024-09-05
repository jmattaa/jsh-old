#ifndef JSH_JSH_H
#define JSH_JSH_H

#include <ncurses.h>
#include <stdlib.h>
#include "../brain/include/execute.h"

#define jsh_ctrl(x) ((x) & 0x1f)

typedef struct {
    char current_command[1024];
    size_t current_command_s;
    int ch;
    int running;
} jshell;

jshell* jsh_init();
void jsh_main(jshell* jsh);
void jsh_destroy(jshell* jsh);

#endif
