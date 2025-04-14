/*
** EPITECH PROJECT, 2025
** src/utilities/arrays/array_counter
** File description:
** This file contains all the logic for couting elements in array
*/

#include "core/minishell.h"

/*
 * Count the number of elements in a array of char *
 * This array need to be NULL terminated.
 */
size_t array_count_string(char **string_array)
{
    size_t count = 0;

    while (string_array[count] != NULL)
        count++;
    return count;
}
