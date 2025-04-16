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
 * @brief replace the backslash with their coresponding ascii character
 *
 * @param arg the argument
 * @return char* the argument with the backslash changed
 */
static char *replace_backslash(IN char *arg)
{
    char *tmp;

    tmp = my_strreplace(arg, "\\n", "\n");
    free(arg);
    arg = my_strreplace(tmp, "\\a", "\a");
    free(tmp);
    tmp = my_strreplace(arg, "\\e", "\e");
    free(arg);
    arg = my_strreplace(tmp, "\\f", "\f");
    free(tmp);
    tmp = my_strreplace(arg, "\\r", "\r");
    free(arg);
    arg = my_strreplace(tmp, "\\t", "\t");
    free(tmp);
    tmp = my_strreplace(arg, "\\v", "\v");
    free(arg);
    arg = my_strreplace(tmp, "\\\\", "\\");
    free(tmp);
    return arg;
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