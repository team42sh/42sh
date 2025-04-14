/*
** EPITECH PROJECT, 2024
** my_put_nbr
** File description:
** A function that prints a given number
*/

#include <stdarg.h>
#include "my_printf.h"

static int my_put_nbr_loop(unsigned long long nb, int print)
{
    int digit;
    int char_count = 0;

    if (nb > 0) {
        digit = nb % 10;
        nb = (nb - digit) / 10;
        char_count += my_put_nbr_loop(nb, print) + 1;
        my_putchar(48 + digit, print);
    }
    return char_count;
}

int my_put_nbr(unsigned long long nb, int print)
{
    if (nb == 0) {
        my_putchar('0', print);
        return 1;
    }
    return my_put_nbr_loop(nb, print);
}
