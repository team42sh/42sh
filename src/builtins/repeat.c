/*
** EPITECH PROJECT, 2025
** src/builtins/repeat
** File description:
** Builtin repeat
*/

#include "core/minishell.h"

/**
 * @brief Create the node.
 *
 * @param token         The token
 *
 * @return The node.
 */
static ast_node_t *make_repeat_node(IN token_t *token)
{
    ast_node_t *node = malloc(sizeof(ast_node_t));

    node->token = token;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/**
 * @brief Create the token.
 *
 * @param argv          ARGV
 *
 * @return The token.
 */
static token_t *make_repeat_token(IN char **argv)
{
    token_t *token = malloc(sizeof(token_t));

    token->token_type = TOKEN_COMMAND;
    token->next = NULL;
    token->data._argv = &argv[2];
    return token;
}

/**
 * @brief Repeat builtin.
 *
 * @param argv          ARGV
 *
 * @return The exitcode.
 */
exitcode_t repeat_command(IN char **argv)
{
    int repeat_times = 0;
    token_t *token = NULL;
    ast_node_t *node = NULL;

    if (argv[1] == NULL || argv[2] == NULL)
        return print_err("repeat: Too few arguments.\n");
    if (!is_only_numbers(argv[1]))
        return print_err("repeat: Badly formed number.\n");
    repeat_times = my_atoi(argv[1]);
    token = make_repeat_token(argv);
    node = make_repeat_node(token);
    for (int i = 0; i < repeat_times; i++)
        execute_command_fork(node);
    free(node);
    free(token);
    return OK_OUTPUT;
}
