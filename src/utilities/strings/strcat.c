/*
** EPITECH PROJECT, 2025
** src/utilities/strings/str_cat
** File description:
** Str cat functions
*/

#include "core/minishell.h"

/*
 * Add the src in the dest at the end. The dest needs to be malloc at the right
 * size. Adds at most n characters from src and put null-termination.
 * Returns the dest pointer.
 */
char *my_strncat(char *dest, char const *src, size_t n)
{
    char *tmp = dest;
    size_t i = 0;

    while (*tmp)
        tmp++;
    while (*src && i < n) {
        *tmp = *src;
        tmp++;
        src++;
        i++;
    }
    *tmp = '\0';
    return dest;
}

/*
 * Add the src in the dest at the end. The dest need to be malloc at the right
 * size.
 * Adding a '\0' and return it.
 */
char *my_strcat(char *dest, char const *src)
{
    char *tmp = dest;

    while (*tmp)
        tmp++;
    while (*src) {
        *tmp = *src;
        tmp++;
        src++;
    }
    *tmp = '\0';
    return dest;
}
