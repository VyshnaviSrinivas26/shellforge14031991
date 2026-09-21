#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtin.h"

int is_builtin(Command *cmd)
{
    if (cmd == NULL || cmd->argc == 0)
        return 0;

    if (strcmp(cmd->argv[0], "cd") == 0)
        return 1;

    if (strcmp(cmd->argv[0], "pwd") == 0)
        return 1;

    if (strcmp(cmd->argv[0], "echo") == 0)
        return 1;

    if (strcmp(cmd->argv[0], "exit") == 0)
        return 1;

    return 0;
}

static int builtin_cd(Command *cmd)
{
    char *dir;

    if (cmd->argc == 1)
    {
        dir = getenv("HOME");

        if (dir == NULL)
        {
            fprintf(stderr, "cd: HOME not set\n");
            return 0;
        }
    }
    else if (cmd->argc == 2)
    {
        dir = cmd->argv[1];
    }
    else
    {
        fprintf(stderr, "cd: too many arguments\n");
        return 0;
    }

    if (chdir(dir) != 0)
    {
        perror("cd");
        return 0;
    }

    return 0;
}

static int builtin_pwd(Command *cmd)
{
    char *buffer;

    if (cmd->argc > 1)
    {
        fprintf(stderr, "pwd: too many arguments\n");
        return 0;
    }

    buffer = getcwd(NULL, 0);

if (buffer == NULL)
{
    perror("pwd");
    return 0;
}

printf("2500032581\n");
printf("%s\n", buffer);

free(buffer);

return 0;
}

static int builtin_echo(Command *cmd)
{
    for (int i = 1; i < cmd->argc; i++)
    {
        printf("%s", cmd->argv[i]);

        if (i < cmd->argc - 1)
            printf(" ");
    }

    printf("\n");

    return 0;
}

static int builtin_exit(Command *cmd)
{
    if (cmd->argc > 1)
    {
        fprintf(stderr, "exit: too many arguments\n");
        return 0;
    }

    return 1;
}

int execute_builtin(Command *cmd)
{
    if (cmd == NULL || cmd->argc == 0)
        return 0;

    if (strcmp(cmd->argv[0], "cd") == 0)
        return builtin_cd(cmd);

    if (strcmp(cmd->argv[0], "pwd") == 0)
        return builtin_pwd(cmd);

    if (strcmp(cmd->argv[0], "echo") == 0)
        return builtin_echo(cmd);

    if (strcmp(cmd->argv[0], "exit") == 0)
        return builtin_exit(cmd);

    return 0;
}
