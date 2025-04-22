/*
** EPITECH PROJECT, 2025
** src/core/parsing/ast_free
** File description:
** This file contains functions for freeing AST
*/

#include "core/minishell.h"

void free_ast(ast_node_t *ast)
{
    if (ast == NULL)
        return;
    if (ast->left != NULL)
        free_ast(ast->left);
    if (ast->right != NULL)
        free_ast(ast->right);
    free(ast);
}

void free_asts(ast_command_t *asts)
{
    if (asts == NULL)
        return;
    for (int i = 0; i < asts->count; i++) {
        free_ast(asts->commands[i]);
    }
    free_null_check(asts->commands);
    free_null_check(asts);
}
