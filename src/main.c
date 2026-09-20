#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "expand.h"

int main(void)
{
    printf("=====================================\n");
    printf("             Shellforge\n");
    printf("    A Unix Style Shell written in C\n");
    printf("=====================================\n");

    printf("Welcome to Milestone2\n\n");

    while (1)
    {
        char *line =
            readline("shellforge$ ");

        if (line == NULL)
        {
            printf("\nGoodbye!\n");
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

                pipeline_free(&pipeline);
            }
        }

        if (strcmp(line, "exit") == 0)
        {
            token_list_free(&list);

            free(line);

            printf("Exiting...\n");

            break;
        }

        token_list_free(&list);

        free(line);
    }

    return 0;
}
