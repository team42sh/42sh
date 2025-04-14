/*
** EPITECH PROJECT, 2024
** print_result
** File description:
** Functions to handle width, precision and other flags of my_printf
*/

#include "my_printf.h"

static int get_real_precision(int precision)
{
    if (precision == -2)
        return 1;
    return precision;
}

int is_left_alligned(formatter_t *format)
{
    if (is_in_str('-', format->flags) != 0)
        return 1;
    return 0;
}

int show_sign(int positive, formatter_t *format)
{
    if (positive == 1) {
        if (is_in_str('+', format->flags)) {
            my_putchar('+', 1);
            return 1;
        }
        if (is_in_str(' ', format->flags)) {
            my_putchar(' ', 1);
            return 1;
        }
    } else {
        my_putchar('-', 1);
        return 1;
    }
    return 0;
}

int show_width_int(formatter_t *format, int actual_char, int positive)
{
    char width_char = ' ';
    int jump_char = 0;
    int sign_active = 0;

    format->precision = get_real_precision(format->precision);
    if (is_in_str('0', format->flags)) {
        width_char = '0';
        jump_char += show_sign(positive, format);
    }
    if (is_in_str('+', format->flags) || is_in_str(' ', format->flags) ||
        !positive)
        sign_active = 1;
    for (int i = 0; i < format->width - sign_active -
        MAX(format->precision, actual_char); i++)
        jump_char += my_putchar(width_char, 1);
    if (!is_in_str('0', format->flags) && !is_left_alligned(format))
        jump_char += show_sign(positive, format);
    for (int i = 0; i < format->precision - actual_char; i++)
        jump_char += my_putchar('0', 1);
    return jump_char;
}

int show_width_double(formatter_t *format, int actual_char, int positive)
{
    char width_char = ' ';
    int jump_char = 0;
    int sign_active = 0;

    format->precision = get_real_precision(format->precision);
    if (is_in_str('0', format->flags)) {
        width_char = '0';
        jump_char += show_sign(positive, format);
    }
    if (is_in_str('+', format->flags) || is_in_str(' ', format->flags) ||
        !positive)
        sign_active = 1;
    for (int i = 0; i < format->width - sign_active -
        MAX(format->precision, actual_char); i++)
        jump_char += my_putchar(width_char, 1);
    if (!is_in_str('0', format->flags) && !is_left_alligned(format))
        jump_char += show_sign(positive, format);
    for (int i = 0; i < format->precision - actual_char; i++)
        jump_char += my_putchar('0', 1);
    return jump_char;
}

int show_width_hex(formatter_t *format, int actual_char,
    int big_x, int print_0x)
{
    char width_char = ' ';
    int jump_char = 0;
    int sign_active = 0;

    format->precision = get_real_precision(format->precision);
    if (is_in_str('0', format->flags)) {
        width_char = '0';
        if (print_0x)
            jump_char += my_putstr(big_x ? "0X" : "0x", 1);
    }
    if (print_0x)
        sign_active = 2;
    for (int i = 0; i < format->width - sign_active -
        MAX(format->precision, actual_char); i++)
        jump_char += my_putchar(width_char, 1);
    if (!is_in_str('0', format->flags) && print_0x)
        jump_char += my_putstr(big_x ? "0X" : "0x", 1);
    for (int i = 0; i < format->precision - actual_char; i++)
        jump_char += my_putchar('0', 1);
    return jump_char;
}
