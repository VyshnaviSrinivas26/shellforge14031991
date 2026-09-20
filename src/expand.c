#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expand.h"

void expand_variables(Pipeline *pipeline)
{
    if (pipeline == NULL)
        return;

    for (int i = 0; i < pipeline->count; i++)
    {
        Command *cmd = pipeline->commands[i];

        if (cmd == NULL)
            continue;

        for (int j = 0; j < cmd->argc; j++)
        {
            if (cmd->argv[j] == NULL)
                continue;

            if (cmd->argv[j][0] == '$')
            {
                char *value = getenv(cmd->argv[j] + 1);

                if (value != NULL)
                {
                    char *new_value = malloc(strlen(value) + 1);

                    if (new_value != NULL)
                    {
                        strcpy(new_value, value);
                        free(cmd->argv[j]);
                        cmd->argv[j] = new_value;
                    }
                }
            }
        }
    }
}
