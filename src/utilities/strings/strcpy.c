/*
** EPITECH PROJECT, 2025
** src/strings/strcpy
** File description:
** This file contains function related for copy stuff into another string
*/

#include "core/minishell.h"

/*
 * Copy the content of a string into another one.
 */
char *my_strcpy(char *dest, char const *src)
{
    char *tmp = dest;

    if (dest == NULL || src == NULL)
        return NULL;
    while (*src) {
        *tmp = *src;
        tmp++;
        src++;
    }
    *tmp = '\0';
    return dest;
}

/*
 * Copy up to n characters from the source string into the destination string.
 * If the source string is shorter than n, the destination will be
 * padded with '\0'.
 */
char *my_strncpy(char *dest, char const *src, size_t n)
{
    char *tmp = dest;

    if (dest == NULL || src == NULL)
        return NULL;
    while (n > 0 && *src) {
        *tmp = *src;
        tmp++;
        src++;
        n--;
    }
    *tmp = '\0';
    return dest;
}

/*
 * Copy up to len character and alloc it to return the reference of the string.
 * This function use my_strncpy but + malloc.
 */
char *my_strncpy_alloc(char const *src, size_t n)
{
    char *result;

    if (src == NULL)
        return NULL;
    result = malloc(sizeof(char) * (n + 1));
    if (!result)
        return NULL;
    my_strncpy(result, src, n);
    result[n] = '\0';
    return result;
}
