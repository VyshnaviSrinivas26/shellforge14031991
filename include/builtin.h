#ifndef BUILTIN_H
#define BUILTIN_H

#include "parser.h"

/*
 * Returns 1 if the command is a built-in command.
 * Returns 0 if it is not a built-in command.
 */
int is_builtin(Command *cmd);

/*
 * Execute a built-in command.
 * Returns:
 * 0 - continue shell
 * 1 - exit shell
 */
int execute_builtin(Command *cmd);

#endif
