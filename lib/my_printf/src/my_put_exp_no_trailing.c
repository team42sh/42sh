/*
** EPITECH PROJECT, 2024
** print_double_exp
** File description:
** A function that prints a given double
** with exponent without trailing zeros
*/

#include "my_printf.h"

static int calculate_exponent(double *value)
{
    int exponent = 0;

    while (*value >= 10.0) {
        exponent++;
        *value /= 10.0;
    }
    while (*value <= 1.0) {
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

static int show_result(double value, int precision, int big_e, int print)
{
    int char_count = 0;
    int exponant = calculate_exponent(&value);

    char_count += my_put_double_no_trailing(value, precision, print);
    char_count += show_exponant(exponant, big_e, print);
    return char_count;
}

int my_put_exp_no_trailing(double nb, int precision, int big_e, int print)
{
    double value = nb;
    int char_count = 0;

    if (value < 0)
        value = -value;
    return char_count + show_result(value, precision, big_e, print);
}
