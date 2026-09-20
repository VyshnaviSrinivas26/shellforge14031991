#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "executor.h"

int execute_external(Command *cmd)
{
    pid_t pid;
    int status;

    if (cmd == NULL || cmd->argc == 0)
        return 1;

    /*
     * argv must end with NULL for execvp().
     */
    cmd->argv[cmd->argc] = NULL;

    /*
     * Create child process.
     */
    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    /*
     * Child process.
     */
    if (pid == 0)
    {
        execvp(cmd->argv[0], cmd->argv);

        /*
         * execvp() returns only if an error occurs.
         */
        perror("execvp");
        exit(1);
    }

    /*
     * Parent process waits for child.
     */
    if (waitpid(pid, &status, 0) < 0)
    {
        perror("waitpid");
        return 1;
    }

    return 0;
}
