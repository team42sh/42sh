/*
** EPITECH PROJECT, 2025
** str_utils.c
** File description:
** This file contains some functions for string utils
*/

#include "../../../include/core/minishell.h"

/*
 * Remove the \n for the last character and put it as \0.
 */
void remove_newline(char *str)
{
    int len = my_strlen(str);

    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

/*
 * Compare 2 strings and return the difference for the last different char.
 */
int my_strcmp(char *s1, char *s2)
{
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

/*
 * Compare 2 strings and return the difference for the last different char.
 * Same as strcmp but with a len max.
 */
int my_strncmp(char *s1, char *s2, size_t n)
{
    size_t temp = 0;

    while (*s1 && (*s1 == *s2) && temp < n) {
        s1++;
        s2++;
        temp++;
    }
    if (temp == n)
        return 0;
    return *s1 - *s2;
}

/*
 * Malloc the right size of a string and return his pointer.
 */
char *my_strdup(char const *src)
{
    int len_src = my_strlen(src);
    char *new_str = malloc(sizeof(char) * (len_src + 1));

    if (new_str == NULL)
        return NULL;
    my_strcpy(new_str, src);
    return new_str;
}
