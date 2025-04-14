/*
** EPITECH PROJECT, 2024
** print_char
** File description:
** Functions that prints a given character
** my_printf flags: %c and %%
*/

#include <stdarg.h>
#include <stddef.h>
#include "my_printf.h"

int print_percent_flag(formatter_t *format)
{
    my_putchar('%', 1);
    return format->conv_index;
}

int print_c_flag(va_list *list, formatter_t *format)
{
    int char_count = 0;

    if (is_left_alligned(format))
        my_putchar(va_arg(*list, int), 1);
    for (int i = 0; i < format->width - 1; i++)
        char_count += my_putchar(' ', 1);
    if (!is_left_alligned(format))
        my_putchar(va_arg(*list, int), 1);
    return 1 + char_count;
}
