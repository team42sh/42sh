/*
** EPITECH PROJECT, 2025
** src/core/shell_executer
** File description:
** This file will manage every things to use the tokens
*/

#include "core/minishell.h"
#include "core/parser.h"

/**
 * @brief Free ASTs and token list. Depending on NULL delivered in parameters.
 *
 * @param asts                  ASTs
 * @param list                  The token list
 */
static void free_tokens_ast(OUT ast_command_t *asts, OUT token_list_t *list)
{
    if (asts != NULL)
        free_asts(asts);
    if (list != NULL)
        free_token_list(list);
}

/**
 * @brief Execute every ast seperated by ';'
 *
 * @param asts                  The list of ASTs
 * @param list                  The list of tokens
 *
 * @return The final exit code of all ASTs. The last command.
 */
static exitcode_t execute_each_ast(OUT ast_command_t *asts,
    OUT token_list_t *list)
{
    exitcode_t exec_command_return = 0;

    for (int i = 0; i < asts->count; i++) {
        if (asts->commands[i] == NULL)
            continue;
        format_ast(asts->commands[i]);
        exec_command_return = execute_ast_node(asts->commands[i]);
        if (exec_command_return == CURRENTLY_CHILD)
            break;
    }
    free_tokens_ast(asts, list);
    return exec_command_return;
}

/**
 * @brief Execute 1 AST
 *
 * @param asts                  ASTs
 * @param i                     The index of the AST to execute
 *
 * @return The final exitcode.
 */
exitcode_t execute_ast_node(IN ast_node_t *node)
{
    if (node == NULL)
        return ERROR_OUTPUT;
    switch (node->token->token_type) {
        case TOKEN_PIPE:
            return execute_pipes(node);
        case TOKEN_COMMAND:
            return execute_command_fork(node);
        case TOKEN_RIGHT_REDIRECTION:
        case TOKEN_LEFT_REDIRECTION:
        case TOKEN_RIGHT_APPEND:
        case TOKEN_LEFT_APPEND:
            return execute_redirection(node);
        case TOKEN_AND:
            return execute_and(node);
        case TOKEN_OR:
            return execute_or(node);
        default:
            return ERROR_OUTPUT;
    }
    return ERROR_OUTPUT;
}

/**
 * @brief Execute a binary. Don't do any fork(). But this function need to be
 *        used after a fork.
 *
 * @param argv   The ARGV
 */
exitcode_t exec_binary(IN char **argv)
{
    char *path;
    char **env_array_child;

    if (argv == NULL)
        return CURRENTLY_CHILD;
    path = get_binary_path(argv[0]);
    if (path == NULL)
        exit(print_err("%s: Command not found.\n", argv[0]));
    env_array_child = env_node_to_array(get_shell()->env);
    execve(path, argv, env_array_child);
    show_error_execve(errno, argv);
    free_array_string(env_array_child);
    free_null_check(path);
    get_shell()->last_exit_code = ERROR_OUTPUT;
    exit(ERROR_OUTPUT);
}

/**
 * @brief Execute a builtin. Don't do any fork().
 *
 * @param argv   The ARGV
 *
 * @return The exit code of the builtin.
 */
exitcode_t exec_built_in(IN char **argv)
{
    int builtin_i = 0;

    if (argv == NULL) {
        get_shell()->last_exit_code = 1;
        return 1;
    }
    builtin_i = is_builtin(argv);
    if (builtin_i == -1)
        return -1;
    get_shell()->last_exit_code = BUILTINS[builtin_i].exec_command(argv);
    return get_shell()->last_exit_code;
}

/**
 * @brief Execute a command with the list of tokens.
 *        A list of tokens is for example : "ls -l | grep root"
 *
 * @param list_tokens   The tokens list
 *
 * @return The exit code resulted by the last command executed.
 */
exitcode_t shell_execute(OUT token_list_t *list_tokens)
{
    ast_command_t *asts = NULL;

    if (list_tokens == NULL || list_tokens->count == 0)
        return 0;
    if (find_token_error(list_tokens) == ERROR_OUTPUT) {
        get_shell()->last_exit_code = ERROR_OUTPUT;
        return ERROR_OUTPUT;
    }
    asts = create_tree(list_tokens);
    return execute_each_ast(asts, list_tokens);
}
