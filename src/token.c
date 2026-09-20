#include <stdio.h>
#include <string.h>

#include "token.h"

void token_list_init(token_list_t *list)
{
    if (list == NULL)
        return;

    list->count = 0;

    for (int i = 0; i < MAX_TOKENS; i++)
    {
        list->tokens[i].type = TOKEN_UNKNOWN;
        list->tokens[i].text[0] = '\0';
    }
}

void token_add(token_list_t *list, token_type_t type, const char *text)
{
    if (list == NULL)
        return;

    if (list->count >= MAX_TOKENS)
        return;

    list->tokens[list->count].type = type;

    if (text != NULL)
    {
        strncpy(list->tokens[list->count].text,
                text,
                MAX_TOKEN_LEN - 1);

        list->tokens[list->count].text[MAX_TOKEN_LEN - 1] = '\0';
    }
    else
    {
        list->tokens[list->count].text[0] = '\0';
    }

    list->count++;
}

const char *token_name(token_type_t type)
{
    switch (type)
    {
        case TOKEN_WORD:
            return "WORD";

        case TOKEN_PIPE:
            return "PIPE";

        case TOKEN_INPUT:
            return "INPUT";

        case TOKEN_OUTPUT:
            return "OUTPUT";

        case TOKEN_APPEND:
            return "APPEND";

        case TOKEN_BACKGROUND:
            return "BACKGROUND";

        case TOKEN_END:
            return "END";

        default:
            return "UNKNOWN";
    }
}

void token_print(const token_list_t *list)
{
    if (list == NULL)
        return;

    printf("\n");
    printf("------------ TOKENS ------------\n");

    for (int i = 0; i < list->count; i++)
    {
        printf("%d : %-12s %s\n",
               i,
               token_name(list->tokens[i].type),
               list->tokens[i].text);
    }

    printf("--------------------------------\n");
}
