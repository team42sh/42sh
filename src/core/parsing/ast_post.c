/*
** EPITECH PROJECT, 2025
** src/core/parsing/ast_post
** File description:
** This file contains functions related of the post process when
** creating the AST
*/

#include "core/minishell.h"

static ast_node_t *swap_or_and(OUT ast_node_t *and_node)
{
    ast_node_t *or_node = and_node->left;
    ast_node_t *true_cmd = or_node->left;
    ast_node_t *fail_cmd = or_node->right;
    ast_node_t *succ_cmd = and_node->right;
    ast_node_t *swap = NULL;

    and_node->left = fail_cmd;
    and_node->right = succ_cmd;
    or_node->left = and_node;
    or_node->right = true_cmd;
    swap = or_node->left;
    or_node->left = or_node->right;
    or_node->right = swap;
    return or_node;
}

/**
 * @brief Execute the piority managment in 1 AST.
 *
 * @param head           The head of the AST to process
 *
 * @return The final priority managed AST.
 */
static ast_node_t *ast_priority_node(IN ast_node_t *head)
{
    ast_node_t *tmp = head;

    while (tmp) {
        if (tmp->token->token_type == TOKEN_AND &&
            tmp->left && tmp->left->token->token_type == TOKEN_OR)
            return swap_or_and(tmp);
        tmp = tmp->left;
    }
    return head;
}

/**
 * @brief For each ASTs in the asts structure do the priority managment.
 *
 * @param asts           The ASTs command that contains every AST
 *
 * @return The ASTs
 */
ast_command_t *ast_priority_process(IN ast_command_t *asts)
{
    for (int i = 0; i < asts->count; i++) {
        asts->commands[i] = ast_priority_node(asts->commands[i]);
    }
    return asts;
}
