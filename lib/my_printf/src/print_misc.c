/*
** EPITECH PROJECT, 2024
** print_misc
** File description:
** A function that stores the number of printed character into the given int *
** my_printf flags: %n
*/

#include <stdarg.h>
#include "my_printf.h"

int print_n_flag(va_list *list, int char_count)
{
    int *ptr = va_arg(*list, int *);

    (*ptr) = char_count;
    return 0;
}
