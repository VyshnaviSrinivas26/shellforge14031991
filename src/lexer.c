#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"
#include "token.h"

int lexer(const char *line, token_list_t *list)
{
    int i = 0;

    if (line == NULL || list == NULL)
        return 0;

    token_list_init(list);

    while (line[i] != '\0')
    {
        char buffer[MAX_TOKEN_LEN];
        int j = 0;

        /* Skip spaces */
        if (isspace((unsigned char)line[i]))
        {
            i++;
            continue;
        }

        /* Pipe: | */
        if (line[i] == '|')
        {
            token_add(list, TOKEN_PIPE, "|");
            i++;
            continue;
        }

        /* Input redirection: < */
        if (line[i] == '<')
        {
            token_add(list, TOKEN_INPUT, "<");
            i++;
            continue;
        }

        /* Output redirection: > */
        if (line[i] == '>')
        {
            if (line[i + 1] == '>')
            {
                token_add(list, TOKEN_APPEND, ">>");
                i += 2;
            }
            else
            {
                token_add(list, TOKEN_OUTPUT, ">");
                i++;
            }

            continue;
        }

        /* Background: & */
        if (line[i] == '&')
        {
            token_add(list, TOKEN_BACKGROUND, "&");
            i++;
            continue;
        }

        /* Quoted word */
        if (line[i] == '"' || line[i] == '\'')
        {
            char quote = line[i++];
            j = 0;

            while (line[i] != '\0' &&
                   line[i] != quote &&
                   j < MAX_TOKEN_LEN - 1)
            {
                buffer[j++] = line[i++];
            }

            if (line[i] == quote)
                i++;

            buffer[j] = '\0';

            token_add(list, TOKEN_WORD, buffer);
            continue;
        }

        /* Normal word */
        while (line[i] != '\0' &&
               !isspace((unsigned char)line[i]) &&
               line[i] != '|' &&
               line[i] != '<' &&
               line[i] != '>' &&
               line[i] != '&' &&
               j < MAX_TOKEN_LEN - 1)
        {
            buffer[j++] = line[i++];
        }

        buffer[j] = '\0';

        if (j > 0)
            token_add(list, TOKEN_WORD, buffer);
    }

    token_add(list, TOKEN_END, "END");

    return 1;
}
