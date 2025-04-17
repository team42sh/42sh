/*
** EPITECH PROJECT, 2025
** echo.c
** File description:
** ECHO COMMAND
*/

#include "core/minishell.h"
#include "core/types.h"
#include "my_printf.h"
#include <stddef.h>

/**
 * @brief change the backslash to the real escaped character, part 2
 * @param arg the arg to print
 * @param offset the offset caused by other backslash
 * @return char* the arg changed
 */
static char *put_backslash_two(IN char *arg, OUT int *offset)
{
    switch (*arg) {
        case 'r':
            arg[- *offset] = '\r';
            return arg;
        case 't':
            arg[- *offset] = '\t';
            return arg;
        case 'v':
            arg[- *offset] = '\v';
            return arg;
        case '\\':
            arg[- *offset] = '\\';
            return arg;
        default:
            (*offset)--;
            arg--;
            return arg;
    }
}

/**
 * @brief change the backslash to the real escaped character
 *
 * @param arg the arg to print
 * @param offset the offset caused by other backslash
 * @return char* the arg changed
 */
static char *put_backslash(IN char *arg, OUT int *offset)
{
    switch (*arg) {
        case 'a':
            arg[- *offset] = '\a';
            return arg;
        case 'b':
            arg[- *offset] = '\b';
            return arg;
        case 'e':
            arg[- *offset] = '\e';
            return arg;
        case 'f':
            arg[- *offset] = '\f';
            return arg;
        case 'n':
            arg[- *offset] = '\n';
            return arg;
        default:
            return put_backslash_two(arg, offset);
    }
}

/**
 * @brief replace the backslash with their coresponding ascii character
 *
 * @param arg the argument
 * @return char* the argument with the backslash changed
 */
static char *replace_backslash(IN char *arg)
{
    int offset = 0;
    char *ptr = arg;

    while (*arg != '\0') {
        if (*arg == '\\') {
            arg++;
            offset++;
            arg = put_backslash(arg, &offset);
        } else
            arg[- offset] = *arg;
        arg++;
    }
    arg[- offset] = '\0';
    return ptr;
}

/**
 * @brief The echo command.
 * It print all the arg separated by an espace and a newline at the end
 *
 * @param argv argument of the function
 * @return exitcode_t If an error occured
 */
exitcode_t echo_command(IN char **argv)
{
    int no_newline = 0;

    if (argv == NULL)
        return ERROR_OUTPUT;
    argv++;
    if (*argv != NULL && my_strcmp(*argv, "-n") == 0) {
        no_newline = 1;
        argv++;
    }
    while (*argv != NULL) {
        *argv = replace_backslash(*argv);
        my_printf("%s", *argv);
        if (*(argv + 1) != NULL)
            my_printf(" ");
        argv++;
    }
    if (!no_newline)
        my_printf("\n");
    return OK_OUTPUT;
}
