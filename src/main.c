#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "expand.h"
#include "builtin.h"

int main(void)
{
    printf("=====================================\n");
    printf("             Shellforge\n");
    printf("    A Unix Style Shell written in C\n");
    printf("=====================================\n");

    printf("Welcome to Milestone3\n\n");

    while (1)
    {
        char *line = readline("shellforge$ ");

        if (line == NULL)
        {
            printf("\n");
            break;
        }

        if (strlen(line) == 0)
        {
            free(line);
            continue;
        }

        add_history(line);

        token_list_t list;

        token_list_init(&list);

        if (lexer(line, &list))
        {
            token_list_print(&list);

            Pipeline pipeline;

            if (parse(list.tokens, &pipeline))
            {
                expand_variables(&pipeline);

                pipeline_print(&pipeline);

                /*
                 * Execute built-in commands
                 * directly inside the shell process.
                 */
                if (pipeline.command_count == 1 &&
                    is_builtin(&pipeline.commands[0]))
                {
                    int should_exit =
                        execute_builtin(&pipeline.commands[0]);

                    pipeline_free(&pipeline);
                    token_list_free(&list);
                    free(line);

                    if (should_exit)
                        break;

                    continue;
                }
            }

            pipeline_free(&pipeline);
        }

        token_list_free(&list);

        free(line);
    }

    return 0;
}
