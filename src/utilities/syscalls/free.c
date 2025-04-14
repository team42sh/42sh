/*
** EPITECH PROJECT, 2025
** src/utilities/syscalls/free
** File description:
** This file will contain a version of free where it check if
** the pointer passed in parameter is NULL or not
*/

#include "../../../include/core/minishell.h"

/*
 * This version of free check if the pointer passed in parameter is NULL or not
 * if yes then just don't anything, if not NULL then free and return and set
 * back the pointer to NULL.
 */
void free_null_check(void *pointer)
{
    if (pointer != NULL) {
        free(pointer);
        pointer = NULL;
    }
}
