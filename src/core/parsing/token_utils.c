/*
** EPITECH PROJECT, 2025
** src/core/parsing/token_utils
** File description:
** This file contains functions for getting some help for using tokens
*/

#include "core/minishell.h"

bool is_token_redirection_append(token_t *token)
{
    if (token->token_type == TOKEN_LEFT_APPEND ||
        token->token_type == TOKEN_RIGHT_APPEND)
        return true;
    return false;
}

bool is_token_redirection_simple(token_t *token)
{
    if (token->token_type == TOKEN_LEFT_REDIRECTION ||
        token->token_type == TOKEN_RIGHT_REDIRECTION)
        return true;
    return false;
}
