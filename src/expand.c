#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "expand.h"

static char *expand_string(const char *input)
{
    if (input == NULL)
        return NULL;

    size_t capacity = 256;
    size_t length = 0;

    char *result = malloc(capacity);

    if (result == NULL)
        return NULL;

    result[0] = '\0';

    for (size_t i = 0; input[i] != '\0';)
    {
        if (input[i] == '$')
        {
            i++;

            char variable[128];
            int j = 0;

            while (input[i] != '\0' &&
                   (isalnum((unsigned char)input[i]) ||
                    input[i] == '_'))
            {
                if (j < 127)
                    variable[j++] = input[i];

                i++;
            }

            variable[j] = '\0';

            if (j == 0)
            {
                if (length + 2 >= capacity)
                {
                    capacity *= 2;
                    result = realloc(result, capacity);
                }

                result[length++] = '$';
                result[length] = '\0';

                continue;
            }

            char *value = getenv(variable);

            if (value == NULL)
                value = "";

            size_t value_len = strlen(value);

            while (length + value_len + 1 >= capacity)
            {
                capacity *= 2;
                result = realloc(result, capacity);
            }

            strcpy(result + length, value);

            length += value_len;
        }
        else
        {
            if (length + 2 >= capacity)
            {
                capacity *= 2;
                result = realloc(result, capacity);
            }

            result[length++] = input[i++];

            result[length] = '\0';
        }
    }

    return result;
}

void expand_variables(Pipeline *pipeline)
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
            char *expanded =
                expand_string(cmd->argv[j]);

            if (expanded != NULL)
            {
                free(cmd->argv[j]);

                cmd->argv[j] = expanded;
            }
        }

        if (cmd->input != NULL)
        {
            char *expanded =
                expand_string(cmd->input);

            if (expanded != NULL)
            {
                free(cmd->input);

                cmd->input = expanded;
            }
        }

        if (cmd->output != NULL)
        {
            char *expanded =
                expand_string(cmd->output);

            if (expanded != NULL)
            {
                free(cmd->output);

                cmd->output = expanded;
            }
        }
    }
}
