/*
** EPITECH PROJECT, 2024
** print_float_hex
** File description:
** Functions that prints a given double in an hexadecimal way
** my_printf flags: %a and %A
*/

#include <stdio.h>
#include "my_printf.h"

static int print_result(int j, char *result, int print)
{
    int char_count = 0;

    while (j > 0) {
        j--;
        my_putchar(result[j], print);
        char_count++;
    }
    return char_count;
}

static int my_putnbr_base(int nbr, char const *base, int print)
{
    int base_l = my_strlen(base);
    long n = (long) nbr;
    char result[32];
    int i = 0;

    if (n == 0) {
        my_putchar(base[0], print);
        return 1;
    }
    while (n > 0) {
        result[i] = base[n % base_l];
        n = n / base_l;
        i++;
    }
    return print_result(i, result, print);
}

static int pow_ten(int exponent)
{
    int i = 0;
    int result = 1;

    while (i < exponent) {
        result *= 10;
        i++;
    }
    return result;
}

static int show_raise(int raise, int big_f, int print)
{
    int char_count = 0;

    if (big_f)
        char_count += my_putstr("P", print);
    else
        char_count += my_putstr("p", print);
    if (raise < 0) {
        my_putchar('-', print);
        raise = -raise;
    } else
        my_putchar('+', print);
    char_count += my_put_nbr(raise, print) + 1;
    return char_count;
}

static int get_raise(double nb, double *dec_part)
{
    int raise = 0;
    double local_nb = nb;

    while (local_nb >= 2.0) {
        raise++;
        local_nb /= 2.0;
    }
    while (local_nb < 1.0) {
        raise--;
        local_nb *= 2.0;
    }
    *dec_part = local_nb - (int) local_nb;
    return raise;
}

static int print_float_hex(double nb, int big_f, int print, formatter_t *f)
{
    double dec_part;
    int raise = get_raise(nb, &dec_part);
    int char_count = 0;
    double d16;

    char_count += my_putstr("1.", print);
    for (int prec = 0; prec < f->precision; prec++) {
        d16 = (dec_part * 16.0) + (0.5 / pow_ten(f->precision));
        if (big_f)
            char_count += my_putnbr_base((int)(d16),
                "0123456789ABCDEF", print);
        else
            char_count += my_putnbr_base((int)(d16),
                "0123456789abcdef", print);
        dec_part = d16 - (int)d16;
    }
    char_count += show_raise(raise, big_f, print);
    return char_count;
}

static int handle_error(double arg, int big_f, int print, formatter_t *f)
{
    if (f->precision == -2)
        f->precision = 6;
    if (arg == 0.0 && big_f)
        return my_putstr("0.000000P+0", print);
    if (arg == 0.0 && !big_f)
        return my_putstr("0.000000p+0", print);
    if (arg < 0.0)
        return print_float_hex(- arg, big_f, print, f);
    return print_float_hex(arg, big_f, print, f);
}

static void print_zero_x(double arg, formatter_t *format, int big_a)
{
    if (big_a) {
        show_sign(POS(arg), format);
        my_putstr("0X", 1);
    } else {
        show_sign(POS(arg), format);
        my_putstr("0x", 1);
    }
}

int print_a_flag(va_list *list, formatter_t *format)
{
    double arg = va_arg(*list, double);
    int char_count = handle_error(arg, 0, 0, format) + 2;

    if (is_in_str('0', format->flags))
        print_zero_x(arg, format, 0);
    if (is_left_alligned(format)) {
        if (!is_in_str('0', format->flags))
            print_zero_x(arg, format, 0);
        handle_error(arg, 0, 1, format);
    }
    if (arg < 0 && !is_in_str(' ', format->flags))
        char_count++;
    char_count += show_width_int(format, char_count, 1);
    if (!is_left_alligned(format)) {
        if (!is_in_str('0', format->flags))
            print_zero_x(arg, format, 0);
        handle_error(arg, 0, 1, format);
    }
    return char_count;
}

int print_uppercase_a_flag(va_list *list, formatter_t *format)
{
    double arg = va_arg(*list, double);
    int char_count = handle_error(arg, 1, 0, format) + 2;

    if (is_in_str('0', format->flags))
        print_zero_x(arg, format, 1);
    if (is_left_alligned(format)) {
        if (!is_in_str('0', format->flags))
            print_zero_x(arg, format, 1);
        handle_error(arg, 1, 1, format);
    }
    if (arg < 0 && !is_in_str(' ', format->flags))
        char_count++;
    char_count += show_width_int(format, char_count, 1);
    if (!is_left_alligned(format)) {
        if (!is_in_str('0', format->flags))
            print_zero_x(arg, format, 1);
        handle_error(arg, 1, 1, format);
    }
    return char_count;
}
