#ifndef PARSER_H
#define PARSER_H

#include "token.h"

#define MAX_COMMANDS 20
#define MAX_ARGS 100

typedef struct {
    char *argv[MAX_ARGS];
    int argc;

    char *input;
    char *output;

    int append;
    int background;
} Command;

typedef struct {
    Command *commands[MAX_COMMANDS];
    int count;
} Pipeline;

void pipeline_init(Pipeline *pipeline);
void pipeline_free(Pipeline *pipeline);

int parse(const token_list_t *tokens, Pipeline *pipeline);

void pipeline_print(const Pipeline *pipeline);

#endif
