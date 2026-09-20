#ifndef TOKEN_H
#define TOKEN_H

#define MAX_TOKENS 128
#define MAX_TOKEN_LEN 256
#define MAX_ARGS 64
#define MAX_COMMANDS 16

typedef enum
{
    TOKEN_WORD,
    TOKEN_INPUT,
    TOKEN_OUTPUT,
    TOKEN_APPEND,
    TOKEN_BACKGROUND,
    TOKEN_PIPE,
    TOKEN_END
} TokenType;

typedef struct
{
    TokenType type;
    char *value;
} Token;

typedef struct
{
    Token tokens[MAX_TOKENS];
    int count;
} token_list_t;

void token_list_init(token_list_t *list);

void token_add(token_list_t *list, TokenType type, const char *value);

const char *token_type_name(TokenType type);

void token_print(Token *tokens);

void token_list_print(token_list_t *list);

void token_list_free(token_list_t *list);

#endif
