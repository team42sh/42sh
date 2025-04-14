/*
** EPITECH PROJECT, 2025
** src/core/parsing/ast_print
** File description:
** This file contains functions for printting AST
*/

#include "../../../include/core/minishell.h"

/**
 * @brief Print the depth to see the right indentation of elements.
 *
 * @param depth          The current indentation level
 */
static void
print_depth(int depth)
{
    for (int i = 0; i < depth; i++)
        my_printf("   ");
}

/**
 * @brief Print every child of a AST.
 *
 * @param ast            The AST
 * @param depth          The current indentation level
 */
static void
print_ast_recursive(ast_node_t *ast, int depth, char const *names[])
{
    if (ast == NULL)
        return;
    print_depth(depth);
    if (ast->token->token_type == TOKEN_RIGHT_APPEND ||
        ast->token->token_type == TOKEN_LEFT_APPEND ||
        ast->token->token_type == TOKEN_LEFT_REDIRECTION ||
        ast->token->token_type == TOKEN_RIGHT_REDIRECTION)
        my_printf("* %s = %s\n", names[ast->token->token_type],
            ast->token->data._file);
    if (ast->token->token_type == TOKEN_COMMAND) {
        my_printf("* %s = ", names[ast->token->token_type]);
        print_array(ast->token->data._argv);
    }
    if (ast->token->token_type == TOKEN_SEMI_COLON)
        my_printf("* %s\n", names[ast->token->token_type]);
    if (ast->token->token_type == TOKEN_PIPE)
        my_printf("* %s\n", names[ast->token->token_type]);
    print_ast_recursive(ast->left, depth + 1, names);
    print_ast_recursive(ast->right, depth + 1, names);
}

/**
 * @brief Print a AST
 *
 * @param ast           The AST to print
 */
void
print_ast(ast_node_t *ast)
{
    if (ast == NULL)
        return;
    print_ast_recursive(ast, 0, tokens_name);
}

/**
 * @brief Print every AST of the ast array contains all AST.
 *
 * @param asts          The array of AST
 */
void
print_asts(ast_command_t *asts)
{
    for (int i = 0; i < asts->count; i++) {
        my_printf("\033[1;33m----------------\033[0m\n");
        print_ast(asts->commands[i]);
        my_printf("\033[1;33m----------------\033[0m\n");
    }
}
