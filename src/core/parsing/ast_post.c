/*
** EPITECH PROJECT, 2025
** src/core/parsing/ast_post
** File description:
** This file contains functions related to the post process when
** creating the AST
*/

#include "core/minishell.h"

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
 * @brief Execute the priority management recursively in 1 AST.
 *
 * @param head           The head of the AST to process
 *
 * @return The final priority-managed AST.
 */
static ast_node_t *ast_priority_node(IN ast_node_t *head)
{
    if (!head)
        return NULL;
    head->left = ast_priority_node(head->left);
    head->right = ast_priority_node(head->right);
    if (head->token->token_type == TOKEN_AND &&
        head->left && (head->left->token->token_type == TOKEN_OR ||
        head->left->token->token_type == TOKEN_AND))
        return swap_node(head);
    if (head->token->token_type == TOKEN_PIPE &&
        head->left && (head->left->token->token_type == TOKEN_OR ||
        head->left->token->token_type == TOKEN_AND))
        return swap_node(head);
    if (is_redirection(head->token->token_type) &&
        head->left && (head->left->token->token_type == TOKEN_OR ||
        head->left->token->token_type == TOKEN_AND))
        return swap_node(head);
    return head;
}

/**
 * @brief For each AST in the asts structure do the priority management.
 *
 * @param asts           The ASTs command that contains every AST
 *
 * @return The updated ASTs
 */
ast_command_t *ast_priority_process(IN ast_command_t *asts)
{
    if (!asts)
        return NULL;
    for (int i = 0; i < asts->count; i++) {
        asts->commands[i] = ast_priority_node(asts->commands[i]);
    }
    return asts;
}
