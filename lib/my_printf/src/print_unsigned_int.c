/*
** EPITECH PROJECT, 2024
** print_unsigned_int
** File description:
** Functions that prints a number in a certain base
** my_printf flags: %b, %o, %u, %x and %X
*/

#include <stdarg.h>
#include "my_printf.h"

static int my_putnbr_base_loop(unsigned int nbr, char const *base,
    int base_len, int print)
{
    int char_count = 0;

    if (nbr == 0)
        return 0;
    char_count += my_putnbr_base_loop(nbr / base_len, base, base_len, print);
    char_count += my_putchar(base[nbr % base_len], print);
    return char_count;
}

static int my_putnbr_base(unsigned int nbr, char const *base, int print)
{
    int base_len = my_strlen(base);
    int char_count = 0;

    char_count += my_putnbr_base_loop(nbr, base, base_len, print);
    return char_count;
}

int print_b_flag(va_list *list, formatter_t *format)
{
    int arg = va_arg(*list, unsigned int);
    int char_count = my_putnbr_base(arg, "01", 0);

    if (!is_left_alligned(format))
        char_count += show_width_int(format, char_count, POS(arg));
    my_putnbr_base(arg, "01", 1);
    if (is_left_alligned(format))
        char_count += show_width_int(format, char_count, POS(arg));
    return char_count;
}

int print_o_flag(va_list *list, formatter_t *format)
{
    int arg = va_arg(*list, unsigned int);
    int char_count = my_putnbr_base(arg, "01234567", 0);

    if (!is_left_alligned(format)) {
        if (is_in_str('#', format->flags))
            char_count++;
        char_count += show_width_int(format, char_count, POS(arg));
        if (is_in_str('#', format->flags))
            my_putchar('0', 1);
    }
    if (is_in_str('#', format->flags) && is_left_alligned(format))
        char_count += my_putchar('0', 1);
    my_putnbr_base(arg, "01234567", 1);
    if (is_left_alligned(format))
        char_count += show_width_int(format, char_count, POS(arg));
    return char_count;
}

int print_u_flag(va_list *list, formatter_t *format)
{
    int arg = va_arg(*list, unsigned int);
    int char_count = my_putnbr_base(arg, "0123456789", 0);

    if (!is_left_alligned(format))
        char_count += show_width_int(format, char_count, POS(arg));
    my_putnbr_base(arg, "0123456789", 1);
    if (is_left_alligned(format))
        char_count += show_width_int(format, char_count, POS(arg));
    return char_count;
}

int print_x_flag(va_list *list, formatter_t *format)
{
    double arg = va_arg(*list, unsigned int);
    int char_count;
    int char_count_tmp = 0;

    if (format->precision == -2)
        format->precision = 1;
    char_count = my_putnbr_base(arg, "0123456789abcdef", 0);
    if (!is_left_alligned(format))
        char_count += show_width_hex(format, char_count,
            0, is_in_str('#', format->flags));
    if (is_left_alligned(format) && is_in_str('#', format->flags))
        char_count_tmp += my_putstr("0x", 1);
    my_putnbr_base(arg, "0123456789abcdef", 1);
    if (is_left_alligned(format))
        char_count += show_width_hex(format, char_count,
            0, is_in_str('#', format->flags));
    return char_count + char_count_tmp;
}

int print_uppercase_x_flag(va_list *list, formatter_t *format)
{
    double arg = va_arg(*list, unsigned int);
    int char_count;
    int char_count_tmp = 0;

    if (format->precision == -2)
        format->precision = 1;
    char_count = my_putnbr_base(arg, "0123456789ABCDEF", 0);
    if (!is_left_alligned(format))
        char_count += show_width_hex(format, char_count,
            1, is_in_str('#', format->flags));
    if (is_left_alligned(format) && is_in_str('#', format->flags))
        char_count_tmp += my_putstr("0X", 1);
    my_putnbr_base(arg, "0123456789ABCDEF", 1);
    if (is_left_alligned(format))
        char_count += show_width_hex(format, char_count,
            1, is_in_str('#', format->flags));
    return char_count + char_count_tmp;
}
