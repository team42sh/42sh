/*
** EPITECH PROJECT, 2025
** src/utilities/strings/strcat_valist
** File description:
** This file contains functions for adding string between with an arg list
*/

#include "../../../include/core/minishell.h"

/*
 * This function do a strcat char with multiples args with arg list.
 * The va_list (...) need to be used with string only !
 * The va_list (...) need to be NULL terminated.
 */
char *my_strcat_list(char *dest, ...)
{
    va_list list;
    char *arg = NULL;

    if (dest == NULL)
        return NULL;
    va_start(list, dest);
    arg = va_arg(list, char *);
    while (arg != NULL) {
        my_strcat(dest, arg);
        arg = va_arg(list, char *);
    }
    va_end(list);
    return dest;
}
