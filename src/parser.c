#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

static char *dup_string(const char *src)
{
    char *copy;
    size_t len;

    if (src == NULL)
        return NULL;

    len = strlen(src);

    copy = malloc(len + 1);

    if (copy == NULL)
        return NULL;

    strcpy(copy, src);

    return copy;
}

void pipeline_init(Pipeline *pipeline)
{
    int i;

    if (pipeline == NULL)
        return;

    pipeline->count = 0;

    for (i = 0; i < MAX_COMMANDS; i++)
        pipeline->commands[i] = NULL;
}

void pipeline_free(Pipeline *pipeline)
{
    int i;
    int j;

    if (pipeline == NULL)
        return;

    for (i = 0; i < pipeline->count; i++)
    {
        Command *cmd = pipeline->commands[i];

        if (cmd == NULL)
            continue;

        for (j = 0; j < cmd->argc; j++)
            free(cmd->argv[j]);

        free(cmd->input);
        free(cmd->output);
        free(cmd);

        pipeline->commands[i] = NULL;
    }

    pipeline->count = 0;
}

int parse(const token_list_t *tokens, Pipeline *pipeline)
{
    int i;
    int current;

    if (tokens == NULL || pipeline == NULL)
        return 0;

    pipeline_init(pipeline);

    pipeline->commands[0] = calloc(1, sizeof(Command));

    if (pipeline->commands[0] == NULL)
        return 0;

    pipeline->count = 1;
    current = 0;

    for (i = 0; i < tokens->count; i++)
    {
        const token_t *t = &tokens->tokens[i];
        Command *cmd = pipeline->commands[current];

        switch (t->type)
        {
            case TOKEN_WORD:
            {
                if (cmd->argc >= MAX_ARGS - 1)
                {
                    fprintf(stderr, "Parse error: too many arguments\n");
                    pipeline_free(pipeline);
                    return 0;
                }

                cmd->argv[cmd->argc] = dup_string(t->text);

                if (cmd->argv[cmd->argc] == NULL)
                {
                    pipeline_free(pipeline);
                    return 0;
                }

                cmd->argc++;
                cmd->argv[cmd->argc] = NULL;

                break;
            }

            case TOKEN_INPUT:
            {
                if (i + 1 >= tokens->count ||
                    tokens->tokens[i + 1].type != TOKEN_WORD)
                {
                    fprintf(stderr,
                            "Parse error: expected filename after <\n");
                    pipeline_free(pipeline);
                    return 0;
                }

                i++;

                free(cmd->input);
                cmd->input = dup_string(tokens->tokens[i].text);

                if (cmd->input == NULL)
                {
                    pipeline_free(pipeline);
                    return 0;
                }

                break;
            }

            case TOKEN_OUTPUT:
            {
                if (i + 1 >= tokens->count ||
                    tokens->tokens[i + 1].type != TOKEN_WORD)
                {
                    fprintf(stderr,
                            "Parse error: expected filename after >\n");
                    pipeline_free(pipeline);
                    return 0;
                }

                i++;

                free(cmd->output);
                cmd->output = dup_string(tokens->tokens[i].text);
                cmd->append = 0;

                if (cmd->output == NULL)
                {
                    pipeline_free(pipeline);
                    return 0;
                }

                break;
            }

            case TOKEN_APPEND:
            {
                if (i + 1 >= tokens->count ||
                    tokens->tokens[i + 1].type != TOKEN_WORD)
                {
                    fprintf(stderr,
                            "Parse error: expected filename after >>\n");
                    pipeline_free(pipeline);
                    return 0;
                }

                i++;

                free(cmd->output);
                cmd->output = dup_string(tokens->tokens[i].text);
                cmd->append = 1;

                if (cmd->output == NULL)
                {
                    pipeline_free(pipeline);
                    return 0;
                }

                break;
            }

            case TOKEN_PIPE:
            {
                if (cmd->argc == 0)
                {
                    fprintf(stderr, "Parse error: empty command before pipe\n");
                    pipeline_free(pipeline);
                    return 0;
                }

                if (pipeline->count >= MAX_COMMANDS)
                {
                    fprintf(stderr, "Parse error: too many commands\n");
                    pipeline_free(pipeline);
                    return 0;
                }

                current++;

                pipeline->commands[current] =
                    calloc(1, sizeof(Command));

                if (pipeline->commands[current] == NULL)
                {
                    pipeline_free(pipeline);
                    return 0;
                }

                pipeline->count++;

                break;
            }

            case TOKEN_BACKGROUND:
            {
                cmd->background = 1;
                break;
            }

            case TOKEN_END:
                break;

            case TOKEN_UNKNOWN:
            default:
                fprintf(stderr,
                        "Parse error: unknown token: %s\n",
                        t->text);
                pipeline_free(pipeline);
                return 0;
        }
    }

    if (pipeline->commands[current]->argc == 0)
    {
        fprintf(stderr, "Parse error: empty command\n");
        pipeline_free(pipeline);
        return 0;
    }

    return 1;
}

void pipeline_print(const Pipeline *pipeline)
{
    int i;
    int j;

    if (pipeline == NULL)
        return;

    printf("\n");
    printf("========== PIPELINE ==========\n");

    for (i = 0; i < pipeline->count; i++)
    {
        const Command *cmd = pipeline->commands[i];

        printf("\nCommand %d\n", i + 1);
        printf("------------------------------\n");

        printf("Arguments\n");

        for (j = 0; j < cmd->argc; j++)
        {
            printf("argv[%d] = %s\n",
                   j,
                   cmd->argv[j]);
        }

        if (cmd->input != NULL)
            printf("Input      : %s\n", cmd->input);
        else
            printf("Input      : None\n");

        if (cmd->output != NULL)
            printf("Output     : %s\n", cmd->output);
        else
            printf("Output     : None\n");

        printf("Append     : %s\n",
               cmd->append ? "Yes" : "No");

        printf("Background : %s\n",
               cmd->background ? "Yes" : "No");
    }

    printf("==============================\n");
}
