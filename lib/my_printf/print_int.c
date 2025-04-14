/*
** EPITECH PROJECT, 2024
** print_int
** File description:
** A function that prints a given int
** my_printf flags: %d and %i
*/

#include <stdarg.h>
#include <stdio.h>
#include "my_printf.h"

int print_int(va_list *list, formatter_t *format)
{
    long arg = (long) va_arg(*list, int);
    int char_count = my_put_nbr(ABS(arg), 0);
    int char_count_tmp = 0;

    if (format->precision == -2)
        format->precision = 1;
    if (!is_left_alligned(format))
        char_count += show_width_int(format, char_count, POS(arg));
    if (is_left_alligned(format))
        char_count_tmp += show_sign(POS(arg), format);
    my_put_nbr(ABS(arg), 1);
    if (is_left_alligned(format))
        char_count += show_width_int(format, char_count, POS(arg));
    return char_count + char_count_tmp;
}
