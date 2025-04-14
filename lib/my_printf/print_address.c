/*
** EPITECH PROJECT, 2024
** print_address
** File description:
** A function that prints the address of a given pointer in hexadecimal
** my_printf flags: %p
*/

#include "my_printf.h"

static int show_address_pointer(unsigned long nbr, char const *base,
    int base_len, int print)
{
    int char_count = 0;

    if (nbr <= 0)
        return 0;
    char_count += show_address_pointer(nbr / base_len, base, base_len, print);
    char_count += my_putchar(base[nbr % base_len], print);
    return char_count;
}

static int convert_address(unsigned long nbr, char const *base, int print)
{
    int base_len = my_strlen(base);
    int char_count = 0;

    my_putchar('0', print);
    my_putchar('x', print);
    char_count += show_address_pointer(nbr, base, base_len, print);
    return char_count + 2;
}

int print_address(va_list *list, formatter_t *format)
{
    unsigned long arg = va_arg(*list, unsigned long);
    int char_count = convert_address(arg, "0123456789abcdef", 0);

    if (is_left_alligned(format))
        convert_address(arg, "0123456789abcdef", 1);
    char_count += show_width_int(format, char_count, 1);
    if (!is_left_alligned(format))
        convert_address(arg, "0123456789abcdef", 1);
    return char_count;
}
