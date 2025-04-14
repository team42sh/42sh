/*
** EPITECH PROJECT, 2024
** print_str
** File description:
** A function that prints a given string
** my_printf flags: %s
*/

#include <stdarg.h>
#include "my_printf.h"

static int my_putstr_n(char *str, int n)
{
    int i = 0;
    int char_count = 0;

    while (str[i] != '\0' && (i < n || n == -2)) {
        char_count += my_putchar(str[i], 1);
        i++;
    }
    return char_count;
}

int print_s_flag(va_list *list, formatter_t *format)
{
    char *arg = va_arg(*list, char *);
    int char_count = my_putstr(arg, 0);
    int char_count_tmp = 0;
    int char_init;

    if (format->precision == -2)
        format->precision = char_count;
    char_init = MIN(format->precision, char_count);
    if (is_left_alligned(format))
        char_count = my_putstr_n(arg, char_init);
    for (int i = 0; i < format->width - char_init; i++) {
        char_count_tmp += my_putchar(' ', 1);
    }
    if (!is_left_alligned(format))
        char_count = my_putstr_n(arg, char_init);
    return char_count + char_count_tmp;
}
