#ifndef PARSER_H
#define PARSER_H

#include "token.h"

typedef struct
{
    char *argv[MAX_ARGS];
    int argc;

    char *input;
    char *output;

    int append;
    int background;

} Command;

typedef struct
{
    Command commands[MAX_COMMANDS];
    int command_count;

} Pipeline;

void pipeline_init(Pipeline *pipeline);

int parse(Token *tokens, Pipeline *pipeline);

void pipeline_print(Pipeline *pipeline);

void pipeline_free(Pipeline *pipeline);

#endif

