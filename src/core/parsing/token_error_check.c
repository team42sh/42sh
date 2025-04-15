/*
** EPITECH PROJECT, 2025
** src/core/parsing/token_error_check
** File description:
** This file contains functions for checking the token in differents ways
*/

#include "core/minishell.h"

/**
 * @brief Analyse error for PIPEs.
 *
 * @param token          The token
 *
 * @return The error if there is any.
 */
static syntax_ast_error_t check_pipe(IN token_t *token)
{
    if (token->token_type != TOKEN_PIPE)
        return NO_ERROR;
    if (!token->next || token->next->token_type == TOKEN_PIPE)
        return PIPE_NO_COMMAND;
    if (token->next->token_type == TOKEN_COMMAND && token->next->next &&
        (token->next->next->token_type == TOKEN_LEFT_REDIRECTION ||
        token->next->next->token_type == TOKEN_LEFT_APPEND))
        return AMBIGUOUS_INPUT;
    return NO_ERROR;
}

/**
 * @brief Analyse error for ANDs and ORs.
 *
 * @param token          The token
 *
 * @return The error if there is any.
 */
static syntax_ast_error_t check_and_or(IN token_t *token)
{
    if (token->token_type != TOKEN_AND && token->token_type != TOKEN_OR)
        return NO_ERROR;
    if (!token->next || token->next->token_type != TOKEN_COMMAND)
        return INVALID_COMMAND;
    return NO_ERROR;
}

/**
 * @brief Analyse error for right redirections and left redirections and file.
 *
 * @param token          The token
 *
 * @return The error if there is any.
 */
static syntax_ast_error_t check_redirections(IN token_t *token)
{
    if (is_redirection(token->token_type) && token->data._file == NULL)
        return REDIRECT_NO_NAME;
    if (token->token_type != TOKEN_LEFT_APPEND &&
        token->token_type != TOKEN_LEFT_REDIRECTION &&
        token->token_type != TOKEN_RIGHT_APPEND &&
        token->token_type != TOKEN_RIGHT_REDIRECTION)
        return NO_ERROR;
    if ((token->next && token->next->token_type == TOKEN_LEFT_APPEND) ||
        (token->next && token->next->token_type == TOKEN_LEFT_REDIRECTION) ||
        (token->next && token->next->token_type == TOKEN_RIGHT_REDIRECTION) ||
        (token->next && token->next->token_type == TOKEN_RIGHT_APPEND))
        return AMBIGUOUS_INPUT;
    if (token->next && token->next->token_type != TOKEN_SEMI_COLON)
        return AMBIGUOUS_OUTPUT;
    return NO_ERROR;
}

/**
 * @brief Analyse every possibles errors by passing a token.
 *        Basically if there is a token AND we need to have a command in left.
 *
 * @param token         The token
 *
 * @return The error if there is any.
 */
syntax_ast_error_t analyse_token(IN token_t *token)
{
    syntax_ast_error_t err = 0;

    if (!token)
        return NO_ERROR;
    err = check_pipe(token);
    if (err != NO_ERROR)
        return err;
    err = check_and_or(token);
    if (err != NO_ERROR)
        return err;
    err = check_redirections(token);
    if (err != NO_ERROR)
        return err;
    return NO_ERROR;
}
