/*
** EPITECH PROJECT, 2025
** src/core/parsing/token_error
** File description:
** This file contains functions for managing and showing error messages
** depending on syntax errors in the token list.
*/

#include "core/minishell.h"
#include "core/parser.h"
#include "core/types.h"

static int show_error_parsing(syntax_ast_error_t error_index)
{
    switch (error_index) {
        case REDIRECT_NO_COMMAND:
        case INVALID_COMMAND:
        case PIPE_NO_COMMAND:
            return print_err("Invalid null command.\n");
        case REDIRECT_NO_NAME:
            return print_err("Missing name for redirect.\n");
        case AMBIGUOUS_OUTPUT:
            return print_err("Ambiguous output redirect.\n");
        case AMBIGUOUS_INPUT:
            return print_err("Ambiguous input redirect.\n");
        case NO_ERROR:
            return OK_OUTPUT;
    }
    return OK_OUTPUT;
}

static int check_head_token(token_list_t *tokens)
{
    if (tokens != NULL && tokens->head != NULL &&
        tokens->head->token_type != TOKEN_COMMAND &&
        tokens->head->token_type != TOKEN_SEMI_COLON) {
        show_error_parsing(INVALID_COMMAND);
        return ERROR_OUTPUT;
    }
    return OK_OUTPUT;
}

static int is_valid_next_semi_colon(token_t *token)
{
    if ((token->token_type == TOKEN_SEMI_COLON && token->next != NULL &&
        token->next->token_type != TOKEN_SEMI_COLON &&
        token->next->token_type != TOKEN_COMMAND))
        return 0;
    return 1;
}

static syntax_ast_error_t get_and_show_error_token(token_t *token)
{
    syntax_ast_error_t error = NO_ERROR;

    error = analyse_token(token);
    show_error_parsing(error);
    return error;
}

bool find_token_error(token_list_t *tokens)
{
    syntax_ast_error_t error = NO_ERROR;

    if (tokens == NULL)
        return ERROR_OUTPUT;
    if (check_head_token(tokens) == ERROR_OUTPUT)
        return INVALID_COMMAND;
    for (token_t *token = tokens->head; token != NULL; token = token->next) {
        error = get_and_show_error_token(token);
        if (error != NO_ERROR)
            break;
        if (!is_valid_next_semi_colon(token)) {
            error = INVALID_COMMAND;
            break;
        }
    }
    if (error != NO_ERROR)
        free_token_list(tokens);
    return error != NO_ERROR ? ERROR_OUTPUT : OK_OUTPUT;
}
