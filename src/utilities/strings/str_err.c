/*
** EPITECH PROJECT, 2025
** src/utilities/strings/str_err
** File description:
** This file contains some functions for printing message in the stderr fd
*/

#include "../../../include/core/minishell.h"

/*
 * Handing of %s in stderr output.
 */
static int handle_s(va_list *list)
{
    char *temp = va_arg(*list, char *);

    write(2, temp, my_strlen(temp));
    return 1;
}

/*
 * Handing of %d in stderr output.
 */
static int handle_d(va_list *list)
{
    char *temp = NULL;

    temp = my_itoa(va_arg(*list, int));
    write(2, temp, my_strlen(temp));
    free(temp);
    return 1;
}

/*
 * Handling the simple character, write it and return 1 to continue the loop.
 */
static int print_simple_char(char *message, int i)
{
    if (message[i] != '%') {
        write(2, &message[i], 1);
        return 1;
    }
    return 0;
}

/*
 * Print a simple message in the stderr output.
 * This message can take only 2 format at the moment :
 * - %s
 * - %d
 * This print cannot handle any kind of formatting.
 * Return ERROR_OUTPUT because it's the standard error message.
 */
int print_err(char *message, ...)
{
    va_list list;

    va_start(list, message);
    for (int i = 0; message[i]; i++) {
        if (print_simple_char(message, i) == 1)
            continue;
        switch (message[i + 1]) {
            case '\0':
                break;
            case 'd':
                i += handle_d(&list);
                break;
            case 's':
                i += handle_s(&list);
                break;
        }
    }
    va_end(list);
    return ERROR_OUTPUT;
}
