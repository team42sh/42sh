/*
** EPITECH PROJECT, 2025
** src/core/parsing/string_until_operator
** File description:
** This file contains functions to get the string until a operator
*/

#include "core/minishell.h"

/**
 * @brief The character is an operator or no?
 *
 * @param c      The character
 *
 * @return True or false.
 */
bool
is_operator(char c)
{
    for (int i = 0; i < my_strlen(OPERATOR_STRINGS); i++)
        if (c == OPERATOR_STRINGS[i])
            return true;
    return false;
}

/**
 * @brief Get the string until a operator is reached or \0.
 * Handle the quotes.
 *
 * @param line  The line.
 *
 * @return The string.
 */
char *
get_string_until_operator(char *line)
{
    int len = 0;
    char *new_string = NULL;
    int in_quotes = 0;

    while (line[len] != '\0') {
        if (is_quote_delimiter(line[len], len > 0 ? line[len - 1] : 0))
            in_quotes = !in_quotes;
        if (is_operator(line[len]) && in_quotes == 0)
            break;
        len++;
    }
    new_string = malloc(sizeof(char) * (len + 1));
    my_strncpy(new_string, line, len);
    return new_string;
}

/**
 * @brief Get the right token by passing char i and char i + 1.
 *
 * @param start_char    The char i
 * @param next_char     The char i + 1
 *
 * @return The token.
 */
token_type_t
get_redirect_token(char start_char, char next_char)
{
    if (start_char == '>') {
        if (next_char == '>')
            return TOKEN_RIGHT_APPEND;
        return TOKEN_RIGHT_REDIRECTION;
    }
    if (start_char == '<') {
        if (next_char == '<')
            return TOKEN_LEFT_APPEND;
        return TOKEN_LEFT_REDIRECTION;
    }
    return TOKEN_END;
}

/**
 * @brief Get the right token by passing char i and char i + 1.
 *
 * @param start_char    The char i
 * @param next_char     The char i + 1
 *
 * @return The token.
 */
token_type_t
get_operator_token(char start_char, char next_char)
{
    if (start_char == ';')
        return TOKEN_SEMI_COLON;
    if (start_char == '|') {
        if (next_char == '|') {
            return TOKEN_OR;
        } else
            return TOKEN_PIPE;
    }
    if (start_char == '&') {
        if (next_char == '&') {
            return TOKEN_AND;
        } else
            return TOKEN_BACKGROUND;
    }
    return TOKEN_END;
}
