/*
** EPITECH PROJECT, 2024
** prin_double_no_trailing
** File description:
** Functions that prints a given double with no trailing zeros
** my_printf flags: %g and %G
*/

#include <stdarg.h>
#include "my_printf.h"

static int handle_special(double nb, int big_f, int print)
{
    int char_count = 0;

    if (nb > 1.7976931348623157e+308 || nb < -1.7976931348623157e+308) {
        if (big_f)
            my_putstr("INF", print);
        else
            my_putstr("inf", print);
        return char_count + 3;
    }
    if (nb != nb) {
        if (big_f)
            my_putstr("NAN", print);
        else
            my_putstr("nan", print);
        return char_count + 3;
    }
    return -1;
}

static int calculate_exponent(double nb)
{
    int exponent = 0;

    if (nb < 0)
        nb = -nb;
    if (nb == 0.0)
        return 0;
    while (nb >= 10.0) {
        exponent++;
        nb /= 10.0;
    }
    while (nb <= 1.0) {
        exponent--;
        nb = nb * 10.0;
    }
    return exponent;
}

static int handle_exponent(double arg, int big_g, int print)
{
    int handle_return = handle_special(arg, big_g, print);
    int exponent;

    if (handle_return != -1)
        return handle_return;
    exponent = calculate_exponent(arg);
    if (exponent >= -4 && exponent < 6)
        return my_put_double_no_trailing(arg, 6, print);
    else
        return my_put_exp_no_trailing(arg, 6, big_g, print);
}

int print_uppercase_g_flag(va_list *list, formatter_t *format)
{
    double arg = va_arg(*list, double);
    int char_count = handle_exponent(arg, 1, 0);

    if (is_in_str('0', format->flags)) {
        show_sign(POS(arg), format);
    }
    if (is_left_alligned(format)) {
        if (!is_in_str('0', format->flags))
            show_sign(POS(arg), format);
        handle_exponent(arg, 1, 1);
    }
    if (arg < 0 && !is_in_str(' ', format->flags))
        char_count++;
    char_count += show_width_int(format, char_count, 1);
    if (!is_left_alligned(format)) {
        if (!is_in_str('0', format->flags))
            show_sign(POS(arg), format);
        handle_exponent(arg, 1, 1);
    }
    return char_count;
}

int print_g_flag(va_list *list, formatter_t *format)
{
    double arg = va_arg(*list, double);
    int char_count = handle_exponent(arg, 0, 0);

    if (is_in_str('0', format->flags)) {
        show_sign(POS(arg), format);
    }
    if (is_left_alligned(format)) {
        if (!is_in_str('0', format->flags))
            show_sign(POS(arg), format);
        handle_exponent(arg, 0, 1);
    }
    if (arg < 0 && !is_in_str(' ', format->flags))
        char_count++;
    char_count += show_width_int(format, char_count, 1);
    if (!is_left_alligned(format)) {
        if (!is_in_str('0', format->flags))
            show_sign(POS(arg), format);
        handle_exponent(arg, 0, 1);
    }
    return char_count;
}
