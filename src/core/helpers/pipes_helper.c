/*
** EPITECH PROJECT, 2025
** src/core/helpers/pipes_helper
** File description:
** This file contains functions for pipes
*/

#include "core/minishell.h"
#include "core/types.h"

/**
 * @brief Regroup of small instructions to dup the right FD.
 *
 * @param pipes[2]       The array of pipes PIPE_READ and PIPE_WRITE
 * @param write          The bool to know if we dup the STDIN or STDOUT
 *
 * @return The return value of dup2.
 */
static int
dup_fd_read_write(int pipes[2], bool write)
{
    int return_value = 0;

    if (write) {
        close(pipes[PIPE_READ]);
        return_value = dup2(pipes[PIPE_WRITE], STDOUT_FILENO);
        close(pipes[PIPE_WRITE]);
    } else {
        close(pipes[PIPE_WRITE]);
        return_value = dup2(pipes[PIPE_READ], STDIN_FILENO);
        close(pipes[PIPE_READ]);
    }
    return return_value;
}

static void
get_argv_pipe_info(ast_node_t *node, char ***argv, bool *is_bt)
{
    ast_node_t *cmd_node = NULL;

    if (node->token->token_type == TOKEN_PIPE)
        return;
    if (is_redirection(node->token->token_type)) {
        cmd_node = node->left;
        if (cmd_node && cmd_node->token->token_type == TOKEN_COMMAND) {
            *argv = cmd_node->token->data._argv;
            *is_bt = (is_builtin(*argv) != -1);
        }
    } else {
        *argv = node->token->data._argv;
        *is_bt = (is_builtin(*argv) != -1);
    }
}

/**
 * @brief Execute a simple command with STDOUT redirection (LEFT PART).
 *
 * @param left_node      The LEFT node in the AST (a command)
 * @param pipes[2]       The pipes array
 * @param pids           The pids array
 *
 * @return The exitcode of the command.
 */
static int
execute_simple_stdout(ast_node_t *left_node, int pipes[2], int *pids)
{
    char **argv = left_node->token->data._argv;
    bool is_argv_builtin = (is_builtin(argv) != -1);

    pids[LEFT] = fork();
    if (pids[LEFT] == 0) {
        dup_fd_read_write(pipes, true);
        if (is_argv_builtin)
            exec_built_in(argv);
        else
            exec_binary(argv);
        return CURRENTLY_CHILD;
    }
    close(pipes[PIPE_WRITE]);
    return OK_OUTPUT;
}

/**
 * @brief Execute a simple command with STDIN redirection (RIGHT PART).
 *
 * @param right_node            The RIGHT node in the AST (a command)
 * @param pipes[2]              The pipes array
 * @param pids                  The pids array
 *
 * @return The exitcode of the command.
 */
static int
execute_simple_stdin(ast_node_t *right_node, int pipes[2], int *pids)
{
    char **argv = right_node->token->data._argv;
    bool is_argv_builtin = (is_builtin(argv) != -1);
    int stdin_save = dup(STDIN_FILENO);

    if (is_argv_builtin) {
        dup2(pipes[PIPE_READ], STDIN_FILENO);
        exec_built_in(argv);
        dup2(stdin_save, STDIN_FILENO);
        close(stdin_save);
        close(pipes[PIPE_READ]);
    } else {
        pids[RIGHT] = fork();
        if (pids[RIGHT] == 0) {
            dup_fd_read_write(pipes, false);
            exec_binary(argv);
            return CURRENTLY_CHILD;
        }
        close(pipes[PIPE_READ]);
    }
    return OK_OUTPUT;
}

/**
 * @brief Execute a redirection as the left part of a pipe
 *
 * @param left_node      The LEFT node in the AST (a redirection)
 * @param pipes[2]       The pipes array
 * @param pids           The pids array
 *
 * @return The exitcode of the command
 */
static int
execute_redirection_stdout(ast_node_t *left_node, int pipes[2], int *pids)
{
    int saved_stdout = 0;

    pids[LEFT] = fork();
    if (pids[LEFT] == 0) {
        close(pipes[PIPE_READ]);
        saved_stdout = dup(STDOUT_FILENO);
        dup2(pipes[PIPE_WRITE], STDOUT_FILENO);
        execute_ast_node(left_node);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
        close(pipes[PIPE_WRITE]);
        return CURRENTLY_CHILD;
    }
    close(pipes[PIPE_WRITE]);
    return OK_OUTPUT;
}

/**
 * @brief Execute the STDOUT redirection part from a pipe (LEFT PART).
 *
 * @param left_node             The LEFT node in the AST
 * @param pipes[2]              The pipes array
 * @param pids                  The pids array
 *
 * @return The exitcode of the command.
 */
static int
execute_stdout_part(ast_node_t *left_node, int pipes[2], int *pids)
{
    if (left_node->token->token_type == TOKEN_PIPE) {
        pids[LEFT] = fork();
        if (pids[LEFT] == 0) {
            dup_fd_read_write(pipes, true);
            execute_pipes(left_node);
            return CURRENTLY_CHILD;
        }
        close(pipes[PIPE_WRITE]);
        return OK_OUTPUT;
    }
    if (is_redirection(left_node->token->token_type))
        return execute_redirection_stdout(left_node, pipes, pids);
    else
        return execute_simple_stdout(left_node, pipes, pids);
}

/**
 * @brief Execute the STDIN redirection part from a pipe (RIGHT PART).
 *
 * @param right_node            The RIGHT node in the AST
 * @param pipes[2]              The pipes array
 * @param pids                  The pids array
 *
 * @return The exitcode of the command.
 */
static int
execute_stdin_part(ast_node_t *right_node, int pipes[2], int *pids)
{
    int stdin_save = dup(STDIN_FILENO);
    int result = 0;

    if (right_node->token->token_type == TOKEN_PIPE) {
        stdin_save = dup(STDIN_FILENO);
        dup2(pipes[PIPE_READ], STDIN_FILENO);
        close(pipes[PIPE_READ]);
        result = execute_pipes(right_node);
        dup2(stdin_save, STDIN_FILENO);
        close(stdin_save);
        return result;
    }
    return execute_simple_stdin(right_node, pipes, pids);
}

/**
 * @brief Get the return value of the pipe.
 *
 * @param is_builtins            Is builtins array?
 * @param status[2]              The return status of both
 * @param pids[2]                The pids of both if not builtins
 * @param node                   The pipe node
 *
 * @return The final exitcode.
 */
static exitcode_t
get_pipes_return(bool is_builtins[2], int status[2], int pids[2],
    ast_node_t *node)
{
    if (!is_builtins[LEFT] && node->left->token->token_type != TOKEN_PIPE)
        waitpid(pids[LEFT], &status[LEFT], 0);
    if (!is_builtins[RIGHT] && node->right->token->token_type != TOKEN_PIPE)
        waitpid(pids[RIGHT], &status[RIGHT], 0);
    if (!is_builtins[RIGHT] && node->right->token->token_type != TOKEN_PIPE)
        get_shell()->last_exit_code = get_wait_status(status[RIGHT]);
    return get_shell()->last_exit_code;
}

/**
 * @brief Execute a pipes.
 *
 * @param node                  The node in the AST of the pipe
 *
 * @return The exitcode of the pipe.
 */
int
execute_pipes(ast_node_t *node)
{
    int pipes[2] = {-1, -1};
    int status[2] = {0, 0};
    pid_t pids[2] = {-1, -1};
    char **argv[2] = {NULL, NULL};
    bool is_builtins[2] = {false, false};

    if (node == NULL)
        return ERROR_OUTPUT;
    get_argv_pipe_info(node->left, &argv[LEFT], &is_builtins[LEFT]);
    get_argv_pipe_info(node->right, &argv[RIGHT], &is_builtins[RIGHT]);
    if (pipe(pipes) == -1)
        return CURRENTLY_CHILD;
    if (execute_stdout_part(node->left, pipes, pids) == CURRENTLY_CHILD)
        return CURRENTLY_CHILD;
    if (execute_stdin_part(node->right, pipes, pids) == CURRENTLY_CHILD)
        return CURRENTLY_CHILD;
    if (node->left->token->token_type != TOKEN_PIPE)
        argv[LEFT] = node->left->token->data._argv;
    return get_pipes_return(is_builtins, status, pids, node);
}
