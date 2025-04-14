/*
** EPITECH PROJECT, 2025
** src/utilities/arrays/print_array
** File description:
** This file contains some functions for printing some types of arrays
*/

#include "../../../include/core/minishell.h"

/*
 * Print the content of a char ** array.
 * Will show with color and print a NULL at the end in white.
 * This function print only array terminated by a NULL pointer.
 */
void print_array(char **array)
{
    int i = 0;

    if (array == NULL) {
        my_printf("[ \033[31;1mNULL \033[0m]");
        return;
    }
    my_printf("[ ");
    while (array[i] != NULL) {
        my_printf("\033[1;35m%s\033[0m, ", array[i]);
        i++;
    }
    if (array[i] == NULL) {
        my_printf("\033[1;39m%s \033[0m", "NULL");
    }
    my_printf("]\n");
}
