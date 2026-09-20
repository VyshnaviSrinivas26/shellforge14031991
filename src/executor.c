#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "executor.h"

/*
 * Execute a single external command.
 *
 * fork()
 *   |
 *   +-- child  -> execvp()
 *   |
 *   +-- parent -> waitpid()
 */
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
         * execvp() returns only when an error occurs.
         */
        perror("execvp");
        exit(1);
    }

    /*
     * Parent waits for child.
     */
    if (waitpid(pid, &status, 0) < 0)
    {
        perror("waitpid");
        return 1;
    }

    return 0;
}


/*
 * Execute a pipeline of commands.
 *
 * Example:
 *
 *     ls | grep .c | wc -l
 *
 * Each pipe connects:
 *
 *     command 1 stdout -> command 2 stdin
 *     command 2 stdout -> command 3 stdin
 */
int execute_pipeline(Pipeline *pipeline)
{
    int i;
    int command_count;
    int previous_pipe_read = -1;

    pid_t pids[MAX_COMMANDS];

    if (pipeline == NULL)
        return 1;

    command_count = pipeline->command_count;

    if (command_count <= 0)
        return 1;

    /*
     * Create and execute every command.
     */
    for (i = 0; i < command_count; i++)
    {
        int pipefd[2];

        /*
         * Create a pipe for every command except the last one.
         */
        if (i < command_count - 1)
        {
            if (pipe(pipefd) < 0)
            {
                perror("pipe");
                return 1;
            }
        }

        /*
         * Create child process.
         */
        pids[i] = fork();

        if (pids[i] < 0)
        {
            perror("fork");
            return 1;
        }

        /*
         * Child process.
         */
        if (pids[i] == 0)
        {
            /*
             * If this is not the first command,
             * connect previous pipe's read end to stdin.
             */
            if (previous_pipe_read != -1)
            {
                if (dup2(previous_pipe_read, STDIN_FILENO) < 0)
                {
                    perror("dup2");
                    exit(1);
                }
            }

            /*
             * If this is not the last command,
             * connect current pipe's write end to stdout.
             */
            if (i < command_count - 1)
            {
                if (dup2(pipefd[1], STDOUT_FILENO) < 0)
                {
                    perror("dup2");
                    exit(1);
                }
            }

            /*
             * Close inherited pipe descriptors.
             */
            if (previous_pipe_read != -1)
                close(previous_pipe_read);

            if (i < command_count - 1)
            {
                close(pipefd[0]);
                close(pipefd[1]);
            }

            /*
             * Make argv NULL terminated.
             */
            pipeline->commands[i].argv[
                pipeline->commands[i].argc
            ] = NULL;

            /*
             * Execute command.
             */
            execvp(
                pipeline->commands[i].argv[0],
                pipeline->commands[i].argv
            );

            /*
             * execvp() returns only on error.
             */
            perror("execvp");
            exit(1);
        }

        /*
         * Parent process.
         */

        /*
         * The old pipe read end is no longer needed
         * by the parent.
         */
        if (previous_pipe_read != -1)
            close(previous_pipe_read);

        /*
         * Save current pipe's read end for the next command.
         */
        if (i < command_count - 1)
        {
            close(pipefd[1]);
            previous_pipe_read = pipefd[0];
        }
        else
        {
            previous_pipe_read = -1;
        }
    }

    /*
     * Wait for all children.
     */
    for (i = 0; i < command_count; i++)
    {
        int status;

        if (waitpid(pids[i], &status, 0) < 0)
        {
            perror("waitpid");
            return 1;
        }
    }

    return 0;
}
