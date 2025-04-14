/*
** EPITECH PROJECT, 2024
** print_double_exp
** File description:
** Functions that prints a given double with exponent
** my_printf flags: %e and %E
*/

#include "my_printf.h"

static int calculate_exponent(double *value)
{
    int exponent = 0;

    while (*value >= 10.0) {
        exponent++;
        *value /= 10.0;
    }
    while (*value < 1.0) {
        exponent--;
        *value *= 10.0;
    }
    return exponent;
}

static int show_exponant(int exponant, int big_e, int print)
{
    int char_count = 2;

    if (big_e) {
        my_putchar('E', print);
    } else {
        my_putchar('e', print);
    }
    if (exponant < 0) {
        exponant = -exponant;
        my_putchar('-', print);
    } else {
        my_putchar('+', print);
    }
    if (exponant < 10) {
        my_putchar('0', print);
        char_count++;
    }
    return char_count + my_put_nbr((unsigned long long) exponant, print);
}

static int handle_error_case(double nb, int precision, int big_e, int print)
{
    if (nb == 0.0) {
        my_putchar('0', print);
        my_putchar('.', print);
        for (int i = 0; i < precision; i++) {
            my_putchar('0', print);
        }
        show_exponant(0, big_e, print);
        return 1;
    }
    return -1;
}

static int show_result(double value, int precision, int big_e, int print)
{
    int char_count = 0;
    int exponant = calculate_exponent(&value);

    char_count += my_put_double(value, precision, print);
    char_count += show_exponant(exponant, big_e, print);
    return char_count;
}

static int my_put_exp(double nb, int precision, int big_e, int print)
{
    double value = nb;
    int char_count = 0;

    if (value < 0)
        value = -value;
    if (handle_error_case(nb, precision, big_e, print) != -1)
        return 6 + precision;
    return char_count + show_result(value, precision, big_e, print);
}

int print_uppercase_e_flag(va_list *list, formatter_t *format)
{
    double arg = va_arg(*list, double);
    int char_count;
    int char_count_tmp = 0;

    if (format->precision == -2)
        format->precision = 6;
    char_count = my_put_exp(arg, format->precision, 1, 0);
    if (!is_left_alligned(format))
        char_count += show_width_double(format, char_count, POS(arg));
    if (is_left_alligned(format))
        char_count_tmp += show_sign(POS(arg), format);
    my_put_exp(arg, format->precision, 1, 1);
    if (is_left_alligned(format))
        char_count += show_width_double(format, char_count, POS(arg));
    return char_count + char_count_tmp;
}

int print_e_flag(va_list *list, formatter_t *format)
{
    double arg = va_arg(*list, double);
    int char_count;
    int char_count_tmp = 0;

    if (format->precision == -2)
        format->precision = 6;
    char_count = my_put_exp(arg, format->precision, 0, 0);
    if (!is_left_alligned(format))
        char_count += show_width_double(format, char_count, POS(arg));
    if (is_left_alligned(format))
        char_count_tmp += show_sign(POS(arg), format);
    my_put_exp(arg, format->precision, 0, 1);
    if (is_left_alligned(format))
        char_count += show_width_double(format, char_count, POS(arg));
    return char_count + char_count_tmp;
}
