/*
** EPITECH PROJECT, 2025
** src/core/parsing/ast
** File description:
** Functions for handling the Abstract Syntax Tree
*/

#include "core/minishell.h"

/**
 * @brief Parse every possible tokens and create the appropriate node.
 *
 * @param asts           The AST
 * @param tokens         The token
 * @param i              The current index in tokens list
 */
static void
pass_possible_tokens(ast_command_t *asts, token_t *tokens, int *i)
{
    if (tokens->token_type == TOKEN_COMMAND && asts->commands[*i] == NULL) {
        asts->commands[*i] = create_ast_command(NULL, tokens);
        return;
    }
    if (tokens->token_type == TOKEN_COMMAND && asts->commands[*i] != NULL)
        asts->commands[*i] = create_ast_command(asts->commands[*i], tokens);
    if (tokens->token_type == TOKEN_PIPE)
        asts->commands[*i] = create_ast_pipe(asts->commands[*i], tokens);
    if (tokens->token_type == TOKEN_LEFT_APPEND ||
        tokens->token_type == TOKEN_RIGHT_APPEND ||
        tokens->token_type == TOKEN_RIGHT_REDIRECTION ||
        tokens->token_type == TOKEN_LEFT_REDIRECTION)
        asts->commands[*i] = create_ast_redirect(asts->commands[*i], tokens);
    if (tokens->token_type == TOKEN_SEMI_COLON) {
        (*i)++;
        asts = add_ast_array(asts);
    }
}

/**
 * @brief Create an AST
 *
 * @param list   The lists of tokens
 *
 * @return The final AST.
 */
ast_command_t *
create_tree(token_list_t *list)
{
    ast_command_t *asts = create_ast_array();
    token_t *tokens = list->head;
    int index = 0;

    if (list->count <= 0)
        return NULL;
    while (tokens != NULL) {
        pass_possible_tokens(asts, tokens, &index);
        tokens = tokens->next;
    }
    return asts;
}
