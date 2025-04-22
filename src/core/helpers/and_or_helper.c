/*
** EPITECH PROJECT, 2025
** src/core/helpers/and_or_helper
** File description:
** This file contains functions for helping with the && and || operators
*/

#include "core/minishell.h"
#include "core/types.h"

/**
 * @brief Execute a AND node.
 *
 * @param node                  The node in the AST of the AND
 *
 * @return The exitcode of the AND.
 */
int execute_and(IN ast_node_t *node)
{
    exitcode_t left_code;
    exitcode_t right_code = -1;

    if (node == NULL)
        return ERROR_OUTPUT;
    left_code = execute_ast_node(node->left);
    if (left_code == OK_OUTPUT)
        right_code = execute_ast_node(node->right);
    get_shell()->last_exit_code = right_code == -1 ? left_code : right_code;
    return get_shell()->last_exit_code;
}

/**
 * @brief Execute a OR node.
 *
 * @param node                  The node in the AST of the OR
 *
 * @return The exitcode of the OR.
 */
int execute_or(IN ast_node_t *node)
{
    exitcode_t left_code;
    exitcode_t right_code = -1;

    if (node == NULL)
        return ERROR_OUTPUT;
    left_code = execute_ast_node(node->left);
    if (left_code != OK_OUTPUT)
        right_code = execute_ast_node(node->right);
    get_shell()->last_exit_code = right_code == -1 ? left_code : right_code;
    return get_shell()->last_exit_code;
}
