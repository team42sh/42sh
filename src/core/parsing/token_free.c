/*
** EPITECH PROJECT, 2025
** src/core/parsing/token_free
** File description:
** This file contains functions for freeing tokens
*/

#include "core/minishell.h"

void free_token(token_t *token)
{
    if (token == NULL)
        return;
    if (token->token_type == TOKEN_COMMAND)
        free_array_string(token->data._argv);
    if (token->token_type == TOKEN_LEFT_APPEND ||
        token->token_type == TOKEN_RIGHT_APPEND ||
        token->token_type == TOKEN_RIGHT_REDIRECTION ||
        token->token_type == TOKEN_LEFT_REDIRECTION)
        free_null_check(token->data._file);
    free_null_check(token);
}

void free_token_list(token_list_t *list)
{
    token_t *tmp_token = NULL;
    token_t *next = NULL;

    if (list == NULL)
        return;
    tmp_token = list->head;
    while (tmp_token != NULL) {
        next = tmp_token->next;
        free_token(tmp_token);
        tmp_token = next;
    }
    free_null_check(list);
}
