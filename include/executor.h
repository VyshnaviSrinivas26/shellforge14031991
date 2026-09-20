#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "parser.h"

/*
 * Execute a single external command.
 */
int execute_external(Command *cmd);

/*
 * Execute multiple commands connected by pipes.
 */
int execute_pipeline(Pipeline *pipeline);

#endif
