#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

static char *copy_string(const char *src)
{
    if (src == NULL)
        return NULL;

    size_t len = strlen(src);

    char *dest = malloc(len + 1);

    if (dest == NULL)
        return NULL;

    strcpy(dest, src);

    return dest;
}

void pipeline_init(Pipeline *pipeline)
{
    pipeline->command_count = 0;

    for (int i = 0; i < MAX_COMMANDS; i++)
    {
        pipeline->commands[i].argc = 0;
        pipeline->commands[i].input = NULL;
        pipeline->commands[i].output = NULL;
        pipeline->commands[i].append = 0;
        pipeline->commands[i].background = 0;

        for (int j = 0; j < MAX_ARGS; j++)
        {
            pipeline->commands[i].argv[j] = NULL;
        }
    }
}

int parse(Token *tokens, Pipeline *pipeline)
{
    pipeline_init(pipeline);

    int token_index = 0;
    int command_index = 0;

    Command *cmd =
        &pipeline->commands[command_index];

    while (tokens[token_index].type != TOKEN_END)
    {
        Token *token =
            &tokens[token_index];

        switch (token->type)
        {
            case TOKEN_WORD:

                if (cmd->argc >= MAX_ARGS - 1)
                {
                    printf("Error: too many arguments\n");
                    return 0;
                }

                cmd->argv[cmd->argc] =
                    copy_string(token->value);

                if (cmd->argv[cmd->argc] == NULL)
                {
                    printf("Error: memory allocation failed\n");
                    return 0;
                }

                cmd->argc++;

                cmd->argv[cmd->argc] = NULL;

                token_index++;

                break;


            case TOKEN_INPUT:

                token_index++;

                if (tokens[token_index].type != TOKEN_WORD)
                {
                    printf("Error: expected filename after <\n");
                    return 0;
                }

                cmd->input =
                    copy_string(tokens[token_index].value);

                if (cmd->input == NULL)
                {
                    printf("Error: memory allocation failed\n");
                    return 0;
                }

                token_index++;

                break;


            case TOKEN_OUTPUT:

                token_index++;

                if (tokens[token_index].type != TOKEN_WORD)
                {
                    printf("Error: expected filename after >\n");
                    return 0;
                }

                cmd->output =
                    copy_string(tokens[token_index].value);

                if (cmd->output == NULL)
                {
                    printf("Error: memory allocation failed\n");
                    return 0;
                }

                cmd->append = 0;

                token_index++;

                break;


            case TOKEN_APPEND:

                token_index++;

                if (tokens[token_index].type != TOKEN_WORD)
                {
                    printf("Error: expected filename after >>\n");
                    return 0;
                }

                cmd->output =
                    copy_string(tokens[token_index].value);

                if (cmd->output == NULL)
                {
                    printf("Error: memory allocation failed\n");
                    return 0;
                }

                cmd->append = 1;

                token_index++;

                break;


            case TOKEN_BACKGROUND:

                cmd->background = 1;

                token_index++;

                break;


            case TOKEN_PIPE:

                if (cmd->argc == 0)
                {
                    printf("Error: empty command before pipe\n");
                    return 0;
                }

                command_index++;

                if (command_index >= MAX_COMMANDS)
                {
                    printf("Error: too many commands in pipeline\n");
                    return 0;
                }

                cmd =
                    &pipeline->commands[command_index];

                token_index++;

                break;


            default:

                printf("Error: unexpected token\n");
                return 0;
        }
    }

    if (cmd->argc == 0 && command_index > 0)
    {
        printf("Error: empty command after pipe\n");
        return 0;
    }

    pipeline->command_count =
        command_index + 1;

    return 1;
}

void pipeline_print(Pipeline *pipeline)
{
    printf("\n========== PIPELINE ==========\n");

    for (int i = 0;
         i < pipeline->command_count;
         i++)
    {
        Command *cmd =
            &pipeline->commands[i];

        printf("\nCommand %d\n", i + 1);

        printf("--------------------------------\n");

        printf("Arguments\n");

        for (int j = 0;
             j < cmd->argc;
             j++)
        {
            printf("argv[%d] = %s\n",
                   j,
                   cmd->argv[j]);
        }

        printf("Input      : %s\n",
               cmd->input ?
               cmd->input : "None");

        printf("Output     : %s\n",
               cmd->output ?
               cmd->output : "None");

        printf("Append     : %s\n",
               cmd->append ?
               "Yes" : "No");

        printf("Background : %s\n",
               cmd->background ?
               "Yes" : "No");

        printf("--------------------------------\n");
    }

    printf("==============================\n");
}

void pipeline_free(Pipeline *pipeline)
{
    for (int i = 0;
         i < pipeline->command_count;
         i++)
    {
        Command *cmd =
            &pipeline->commands[i];

        for (int j = 0;
             j < cmd->argc;
             j++)
        {
            free(cmd->argv[j]);
            cmd->argv[j] = NULL;
        }

        free(cmd->input);
        free(cmd->output);

        cmd->input = NULL;
        cmd->output = NULL;
    }
}
