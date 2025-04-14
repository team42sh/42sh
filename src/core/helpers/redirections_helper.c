/*
** EPITECH PROJECT, 2025
** src/core/helpers/redirections_helper
** File description:
** This file contains functions for handling the redirections
*/

#include "../../../include/core/minishell.h"
#include <stdbool.h>

/**
 * @brief Show the final error message of the open section in redirection.
 *
 * @param file_name     The file to open
 *
 * @return ERROR_OUTPUT.
 */
static int
show_file_open_error(char *file_name)
{
    get_shell()->last_exit_code = ERROR_OUTPUT;
    switch (errno) {
        case ENOENT:
            return print_err("%s: No such file or directory.\n", file_name);
        case EACCES:
            return print_err("%s: Permission denied.\n", file_name);
    }
    return ERROR_OUTPUT;
}

/**
* @brief Right redirection.
*
* @param node           The node of AST
* @param append         Is this an append or not?
*
* @return The final return value of the execution.
*/
static int
redirect_right(ast_node_t *node, bool append)
{
    int fd = -1;
    int return_value = 0;
    int saved_stdout = dup(STDOUT_FILENO);

    if (append)
        fd = open(node->token->data._file, O_WRONLY | O_CREAT | O_APPEND,
            0644);
    else
        fd = open(node->token->data._file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
        return show_file_open_error(node->token->data._file);
    dup2(fd, STDOUT_FILENO);
    return_value = execute_ast_node(node->left);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
    close(fd);
    if (return_value == CURRENTLY_CHILD)
        return CURRENTLY_CHILD;
    return return_value;
}

/**
* @brief Left redirection.
*
* @param node           The node of AST
*
* @return The final return value of the execution.
*/
static int
redirect_left(ast_node_t *node)
{
    int fd = open(node->token->data._file, O_RDONLY);
    int return_value = 0;
    int saved_stdin = dup(STDIN_FILENO);

    if (fd == -1)
        return show_file_open_error(node->token->data._file);
    dup2(fd, STDIN_FILENO);
    return_value = execute_ast_node(node->left);
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
    close(fd);
    if (return_value == CURRENTLY_CHILD)
        return CURRENTLY_CHILD;
    return return_value;
}

/**
 * @brief Writes the here document to a temporary file.
 *
 * @param node           The node of AST
 *
 * @return OK_OUTPUT or ERROR_OUTPUT.
 */
static int
write_here_document(int fd, ast_node_t *node)
{
    size_t buffer_size = 0;
    char *buffer = NULL;

    if (fd == -1)
        return show_file_open_error("");
    my_printf("? ");
    while (getline(&buffer, &buffer_size, stdin) != -1) {
        remove_newline(buffer);
        if (my_strcmp(buffer, node->token->data._file) == 0)
            break;
        write(fd, buffer, my_strlen(buffer));
        write(fd, "\n", 1);
        my_printf("? ");
    }
    free(buffer);
    return OK_OUTPUT;
}

/**
 * @brief Left append redirection (here document).
 *
 * @param node           The node of AST
 *
 * @return The final return value of the execution.
 */
static int
redirect_left_append(ast_node_t *node)
{
    int pipefd[2] = {-1, -1};
    int saved_stdin = dup(STDIN_FILENO);
    int return_value = 0;

    pipe(pipefd);
    write_here_document(pipefd[PIPE_WRITE], node);
    close(pipefd[PIPE_WRITE]);
    dup2(pipefd[PIPE_READ], STDIN_FILENO);
    close(pipefd[PIPE_READ]);
    return_value = execute_ast_node(node->left);
    close(pipefd[PIPE_READ]);
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
    if (return_value == CURRENTLY_CHILD)
        return CURRENTLY_CHILD;
    return return_value;
}

/**
 * @brief Check if node is a redirection node.
 *
 * @param node      The node to check
 *
 * @return TRUE if node is a redirection, FALSE otherwise.
 */
bool
is_redirection(token_type_t type)
{
    return (type == TOKEN_LEFT_REDIRECTION || type == TOKEN_RIGHT_REDIRECTION
            || type == TOKEN_LEFT_APPEND || type == TOKEN_RIGHT_APPEND);
}

/**
 * @brief Execute a redirection.
 *
 * @param node          The node of AST
 *
 * @return The final exit code of the redirection.
 */
int
execute_redirection(ast_node_t *node)
{
    if (node->token->token_type == TOKEN_RIGHT_REDIRECTION)
        return redirect_right(node, false);
    if (node->token->token_type == TOKEN_RIGHT_APPEND)
        return redirect_right(node, true);
    if (node->token->token_type == TOKEN_LEFT_REDIRECTION)
        return redirect_left(node);
    if (node->token->token_type == TOKEN_LEFT_APPEND)
        return redirect_left_append(node);
    return ERROR_OUTPUT;
}
