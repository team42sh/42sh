/*
** EPITECH PROJECT, 2024
** my_put_double_no_trailing
** File description:
** A function that prints a double in a
** decimal way without trailing zeros
*/

#include <stdarg.h>
#include "my_printf.h"

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

static void find_dec_end_idx(double *nb_dec_part, int *end_idx, int precision)
{
    int non_zero_found = 0;

    for (int i = 0; i < precision + 3 && non_zero_found != precision; i++) {
        (*end_idx)++;
        *nb_dec_part *= 10.0;
        if ((int) (*nb_dec_part + 0.5) % 10 != 0)
            non_zero_found++;
        if (i == precision - 1 && non_zero_found == 0)
            break;
    }
    *nb_dec_part += 0.5;
    for (int i = *end_idx; i >= 0; i--) {
        if ((int) *nb_dec_part % 10 == 0)
            (*end_idx)--;
        if ((int) *nb_dec_part % 10 != 0)
            break;
        *nb_dec_part /= 10.0;
    }
}

static int show_dec_part(double *nb_dec_part, int precision, int print)
{
    int char_count = 0;
    int end_idx = -1;

    find_dec_end_idx(nb_dec_part, &end_idx, precision);
    *nb_dec_part /= pow_ten(end_idx);
    if (end_idx >= 0)
        char_count += my_putchar('.', print);
    for (int i = end_idx; i >= 0; i--) {
        char_count += my_putchar((int) (*nb_dec_part) % 10 + '0', print);
        (*nb_dec_part) *= 10.0;
    }
    return char_count;
}

static int display_number(unsigned long long nb_int_part,
    double *nb_dec_part, int precision, int print)
{
    int char_count = 0;

    char_count += my_put_nbr(nb_int_part, print);
    char_count += show_dec_part(nb_dec_part, precision, print);
    return char_count;
}

int my_put_double_no_trailing(double nb, int precision, int print)
{
    unsigned long long nb_int_part;
    double nb_dec_part;
    int char_count = 0;

    if (nb == 0.0)
        return my_putchar('0', print);
    if (nb < 0)
        nb = -nb;
    nb_int_part = (unsigned long long) nb;
    nb_dec_part = nb - nb_int_part;
    if (nb_int_part != 0)
        precision -= count_int_char(nb_int_part);
    if (nb_dec_part + (0.5 / pow_ten(precision)) >= 1.0)
        nb_int_part++;
    return char_count + display_number(nb_int_part,
        &nb_dec_part, precision, print);
}
