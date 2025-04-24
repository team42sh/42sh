/*
** EPITECH PROJECT, 2025
** src/core/parsing/token_utils
** File description:
** This file contains functions for getting some help for using tokens
*/

#include "core/minishell.h"

/**
 * @brief Is token, a redirection append.
 *
 * @param token         The token
 *
 * @return TRUE or FALSE.
 */
bool is_token_redirection_append(IN token_t *token)
{
    if (token == NULL)
        return false;
    if (token->token_type == TOKEN_LEFT_APPEND ||
        token->token_type == TOKEN_RIGHT_APPEND)
        return true;
    return false;
}

/**
 * @brief Is token, a redirection simple.
 *
 * @param token         The token
 *
 * @return TRUE or FALSE.
 */
bool is_token_redirection_simple(IN token_t *token)
{
    if (token == NULL)
        return false;
    if (token->token_type == TOKEN_LEFT_REDIRECTION ||
        token->token_type == TOKEN_RIGHT_REDIRECTION)
        return true;
    return false;
}

/**
 * @brief Is token, a redirection right.
 *
 * @param token         The token
 *
 * @return TRUE or FALSE.
 */
bool is_token_redirection_right(IN token_t *token)
{
    if (token == NULL)
        return false;
    if (token->token_type == TOKEN_RIGHT_REDIRECTION ||
        token->token_type == TOKEN_RIGHT_APPEND)
        return true;
    return false;
}

/**
 * @brief Is token, a redirection left.
 *
 * @param token         The token
 *
 * @return TRUE or FALSE.
 */
bool is_token_redirection_left(IN token_t *token)
{
    if (token == NULL)
        return false;
    if (token->token_type == TOKEN_LEFT_REDIRECTION ||
        token->token_type == TOKEN_LEFT_APPEND)
        return true;
    return false;
}
