#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

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

void token_list_init(token_list_t *list)
{
    if (list == NULL)
        return;

    list->count = 0;

    for (int i = 0; i < MAX_TOKENS; i++)
    {
        list->tokens[i].type = TOKEN_END;
        list->tokens[i].value = NULL;
    }
}

void token_add(token_list_t *list, TokenType type, const char *value)
{
    if (list == NULL)
        return;

    if (list->count >= MAX_TOKENS - 1)
        return;

    list->tokens[list->count].type = type;

    list->tokens[list->count].value =
        copy_string(value);

    list->count++;
}

const char *token_type_name(TokenType type)
{
    switch (type)
    {
        case TOKEN_WORD:
            return "WORD";

        case TOKEN_INPUT:
            return "INPUT";

        case TOKEN_OUTPUT:
            return "OUTPUT";

        case TOKEN_APPEND:
            return "APPEND";

        case TOKEN_BACKGROUND:
            return "BACKGROUND";

        case TOKEN_PIPE:
            return "PIPE";

        case TOKEN_END:
            return "END";

        default:
            return "UNKNOWN";
    }
}

void token_print(Token *tokens)
{
    int i = 0;

    printf("\n------------ TOKENS ------------\n");

    while (tokens[i].type != TOKEN_END)
    {
        printf("%d : %-12s %s\n",
               i,
               token_type_name(tokens[i].type),
               tokens[i].value ? tokens[i].value : "");

        i++;
    }

    printf("%d : END          END\n", i);

    printf("--------------------------------\n");
}

void token_list_print(token_list_t *list)
{
    if (list == NULL)
        return;

    token_print(list->tokens);
}

void token_list_free(token_list_t *list)
{
    if (list == NULL)
        return;

    for (int i = 0; i < list->count; i++)
    {
        free(list->tokens[i].value);
        list->tokens[i].value = NULL;
    }

    list->count = 0;
}
