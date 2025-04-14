/*
** EPITECH PROJECT, 2025
** src/utilities/strings/strtok_reverse
** File description:
** This file contains the same implementation of strtok but in reverse
*/

#include "core/minishell.h"

/*
 * Count the number of character before the until_char.
 * By starting at the end.
 */
int count_char_from_end(const char *str, char c)
{
    int i = 0;
    int len = my_strlen(str);

    while (i < len && str[len - 1 - i] != c)
        i++;
    return i;
}

/*
 * Return the entire string after the last occurrence of until_char.
 * Example : strtok_reverse("foo=bar=baz", '=') --> "baz".
 */
char *my_strtok_reverse(char *string, char until_char)
{
    static char *last_pos = NULL;
    static char *start = NULL;
    char *token_end = NULL;
    char *token_start = NULL;

    if (string != NULL) {
        last_pos = string + my_strlen(string);
        start = string;
    } else if (last_pos == NULL || last_pos <= start)
        return NULL;
    if (last_pos > start && *(last_pos - 1) == until_char)
        last_pos--;
    if (last_pos <= start)
        return NULL;
    token_end = last_pos;
    while (last_pos > start && *(last_pos - 1) != until_char)
        last_pos--;
    token_start = last_pos;
    token_end[0] = '\0';
    return token_start;
}
