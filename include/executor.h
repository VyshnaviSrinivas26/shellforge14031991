#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "parser.h"

/*
 * Execute an external command.
 *
 * Returns:
 * 0 - command executed successfully
 * 1 - error occurred
 */
int execute_external(Command *cmd);

#endif
