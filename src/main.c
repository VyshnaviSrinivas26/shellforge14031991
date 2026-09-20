#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "token.h"
#include "parser.h"
#include "expand.h"

int lexer(const char *line, token_list_t *list);

int main(void)
{
    char *line;

    token_list_t tokens;
    Pipeline pipeline;

    printf("====================================\n");
    printf("          ShellForge\n");
    printf("   A Unix Style Shell written in C\n");
    printf("====================================\n");
    printf("Welcome to Milestone 2\n\n");

    while (1)
    {
        line = readline("shellforge$ ");

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

        if (lexer(line, &tokens))
        {
            token_print(&tokens);

            pipeline_init(&pipeline);

            if (parse(&tokens, &pipeline))
            {
                expand_variables(&pipeline);
                pipeline_print(&pipeline);

                pipeline_free(&pipeline);
            }
        }

        if (strcmp(line, "exit") == 0)
        {
            free(line);
            printf("Exiting...\n");
            break;
        }

        free(line);
    }

    return 0;
}
