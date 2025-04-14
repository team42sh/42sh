/*
** EPITECH PROJECT, 2024
** my_put_double
** File description:
** A function that prints a double in a decimal way
*/

#include <stdarg.h>
#include "my_printf.h"

static int handle_error_case(double nb, int precision, int print)
{
    if (nb == 0.0) {
        my_putchar('0', print);
        my_putchar('.', print);
        for (int i = 0; i < precision; i++) {
            my_putchar('0', print);
        }
        return 1;
    }
    return -1;
}

static int count_int_char(unsigned long long nb)
{
    int current_len = 0;

    if (nb == 0)
        return 1;
    while (nb > 0) {
        nb /= 10;
        current_len++;
    }
    return current_len;
}

static unsigned long long pow_ten(int exp)
{
    unsigned long long result = 1;

    for (int i = 0; i < exp; i++) {
        result *= 10;
    }
    return result;
}

static int show_dec_part(unsigned long long nb_int_part,
    double *nb_dec_part, int precision, int print)
{
    int char_count = count_int_char(nb_int_part);

    *nb_dec_part += 0.5 / pow_ten(precision);
    for (int i = 0; i < precision; i++) {
        *nb_dec_part *= 10;
        char_count += my_putchar((int) (*nb_dec_part) % 10 + '0', print);
    }
    return char_count;
}

int my_put_double(double nb, int precision, int print)
{
    unsigned long long nb_int_part;
    double nb_dec_part;
    int char_count = 0;

    if (handle_error_case(nb, precision, print) != -1)
        return 2 + precision;
    if (nb < 0)
        nb *= -1;
    nb_int_part = (unsigned long long) nb;
    nb_dec_part = nb - nb_int_part;
    if (nb_dec_part + (0.5 / pow_ten(precision)) >= 1.0)
        nb_int_part++;
    my_put_nbr(nb_int_part, print);
    if (precision >= 1)
        char_count += my_putchar('.', print);
    char_count += show_dec_part(nb_int_part, &nb_dec_part, precision, print);
    return char_count;
}
