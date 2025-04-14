/*
** EPITECH PROJECT, 2024
** print_double
** File description:
** Functions that prints a given double
** my_printf flags: %f and %F
*/

#include <stdarg.h>
#include "my_printf.h"

static int is_special(double nb)
{
    if (nb > 1.7976931348623157e+308 || nb < -1.7976931348623157e+308)
        return 1;
    if (nb != nb)
        return 1;
    return 0;
}

static int handle_special(double nb, int big_f, int print, int precision)
{
    int minus = 0;

    if (nb > 1.7976931348623157e+308 || nb < -1.7976931348623157e+308) {
        if (big_f)
            return my_putstr("INF", print) + minus;
        return my_putstr("inf", print) + minus;
    }
    if (nb != nb) {
        if (big_f)
            return my_putstr("NAN", print);
        return my_putstr("nan", print);
    }
    return my_put_double(nb, precision, print);
}

int print_uppercase_f_flag(va_list *list, formatter_t *format)
{
    double arg = va_arg(*list, double);
    int char_count;
    int char_count_tmp = 0;

    if (format->precision == -2)
        format->precision = 6;
    if (is_special(arg))
        format->precision = 3;
    char_count = handle_special(arg, 1, 0, format->precision);
    if (!is_left_alligned(format))
        char_count += show_width_double(format, char_count, POS(arg));
    if (is_left_alligned(format))
        char_count_tmp += show_sign(POS(arg), format);
    handle_special(arg, 1, 1, format->precision);
    if (is_left_alligned(format))
        char_count += show_width_double(format, char_count, POS(arg));
    return char_count + char_count_tmp;
}

int print_f_flag(va_list *list, formatter_t *format)
{
    double arg = va_arg(*list, double);
    int char_count;
    int char_count_tmp = 0;

    if (format->precision == -2)
        format->precision = 6;
    if (is_special(arg))
        format->precision = 3;
    char_count = handle_special(arg, 0, 0, format->precision);
    if (!is_left_alligned(format))
        char_count += show_width_double(format, char_count, POS(arg));
    if (is_left_alligned(format))
        char_count_tmp += show_sign(POS(arg), format);
    handle_special(arg, 0, 1, format->precision);
    if (is_left_alligned(format))
        char_count += show_width_double(format, char_count, POS(arg));
    return char_count + char_count_tmp;
}
