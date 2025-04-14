/*
** EPITECH PROJECT, 2025
** src/core/parsing/ast_actions
** File description:
** This file contains some functions for action in AST
*/

#include "../../../include/core/minishell.h"

/**
 * @brief Create a AST node for piping.
 *
 * @param ast            The AST
 * @param token          The token value
 *
 * @return               The node of the AST.
 */
ast_node_t *
create_ast_pipe(ast_node_t *ast, token_t *token)
{
    ast_node_t *new_ast = malloc(sizeof(ast_node_t));

    new_ast->left = ast;
    new_ast->right = NULL;
    new_ast->token = token;
    return new_ast;
}

/**
 * @brief Create a AST node for redirection.
 *
 * @param ast            The AST
 * @param token          The token value
 *
 * @return               The node of the AST.
 */
ast_node_t *
create_ast_redirect(ast_node_t *ast, token_t *token)
{
    ast_node_t *new_ast = malloc(sizeof(ast_node_t));

    new_ast->left = ast;
    new_ast->right = NULL;
    new_ast->token = token;
    return new_ast;
}

/**
 * @brief Create a AST node for a simple command.
 *
 * @param ast            The AST
 * @param token          The token value
 *
 * @return               The node of the AST.
 */
ast_node_t *
create_ast_command(ast_node_t *ast, token_t *token)
{
    ast_node_t *new_ast = malloc(sizeof(ast_node_t));

    new_ast->left = NULL;
    new_ast->right = NULL;
    new_ast->token = token;
    if (ast == NULL)
        return new_ast;
    if (ast->token->token_type == TOKEN_PIPE ||
        ast->token->token_type == TOKEN_SEMI_COLON)
        ast->right = new_ast;
    else
        ast->left = new_ast;
    return ast;
}
