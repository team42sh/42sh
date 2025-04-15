/*
** EPITECH PROJECT, 2025
** src/core/parsing/ast_post
** File description:
** This file contains functions related of the post process when
** creating the AST
*/

#include "core/minishell.h"
#include "core/parser.h"

static ast_node_t *swap_node(OUT ast_node_t *node)
{
    ast_node_t *left_node = node->left;
    ast_node_t *true_cmd = left_node->left;
    ast_node_t *fail_cmd = left_node->right;
    ast_node_t *succ_cmd = node->right;
    ast_node_t *swap = NULL;

    node->left = fail_cmd;
    node->right = succ_cmd;
    left_node->left = node;
    left_node->right = true_cmd;
    swap = left_node->left;
    left_node->left = left_node->right;
    left_node->right = swap;
    return left_node;
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
            tmp->left && (tmp->left->token->token_type == TOKEN_OR ||
            tmp->left->token->token_type == TOKEN_AND))
            return swap_node(tmp);
        if (tmp->token->token_type == TOKEN_PIPE &&
            tmp->left && (tmp->left->token->token_type == TOKEN_AND ||
            tmp->left->token->token_type == TOKEN_OR))
            return swap_node(tmp);
        if (is_redirection(tmp->token->token_type) &&
            tmp->left && (tmp->left->token->token_type == TOKEN_AND ||
            tmp->left->token->token_type == TOKEN_OR))
            return swap_node(tmp);
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
