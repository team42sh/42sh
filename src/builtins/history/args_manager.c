/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** history
*/

#include "core/minishell.h"
#include "core/builtins.h"
#include "core/builtins/history.h"

static int get_args_number(IN char **argv)
{
    int i = 0;

    while (argv[i] != NULL) {
        i++;
    }
    return i;
}

history_args_t *history_parse_arguments(IN char **argv)
{
    history_args_t *args = calloc(1, sizeof(history_args_t));

    args->lines_count = -1;
    if (get_args_number(argv) > 3) {
        my_printf("history: Too many arguments.\n");
        return args;
    }
    if (argv[1] != NULL && strcmp(argv[1], "-c") == 0) {
        args->clear = 1;
        return args;
    }
    return args;
}
