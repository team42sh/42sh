/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** history
*/

#include "core/minishell.h"
#include "core/builtins.h"
#include "core/builtins/history.h"

/**
 * @brief Get the number of arguments in the argv array.
 *
 * @param argv The arguments to count.
 * @return int The number of arguments.
 */
static int get_args_number(IN char **argv)
{
    int i = 0;

    while (argv[i] != NULL) {
        i++;
    }
    return i;
}

/**
 * @brief Parse the number of lines to read from the history file.
 *
 * @param argv The arguments to parse.
 * @param args The history arguments structure to fill.
 * @return int 0 on success, -1 on failure.
 */
static int history_parse_line_count(IN char **argv, IN history_args_t *args)
{
    if (argv[1] != NULL && argv[1][0] != '-')
        args->lines_count = atoi(argv[1]);
    else if (argv[2] != NULL && argv[2][0] != '-')
        args->lines_count = atoi(argv[2]);
    return 0;
}

/**
 * @brief Parse the argument string and set the corresponding flags in args.
 *
 * @param arg The argument string to parse.
 * @param args The history arguments structure to fill.
 * @return char* The argument string.
 */
static char *parse_argument_string(IN char *arg, IN history_args_t *args)
{
    if (arg[0] != '-')
        return arg;
    for (int i = 1; arg[i] != '\0'; i++) {
        switch (arg[i]) {
            case 'c':
                args->clear = 1;
                break;
            case 'r':
                args->reverse = 1;
                break;
            case 'h':
                args->simple = 1;
                break;
            default:
                my_printf("history: Invalid option: %c\n", arg[i]);
                return NULL;
        }
    }
    return arg;
}

/**
 * @brief Parse the arguments for the history command.
 *
 * @param argv The arguments to parse.
 * @return history_args_t* The parsed arguments.
 */
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
    if (argv[1] != NULL && argv[1][0] == '-')
        parse_argument_string(argv[1], args);
    history_parse_line_count(argv, args);
    return args;
}
