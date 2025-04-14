/*
** EPITECH PROJECT, 2024
** my_putstr
** File description:
** A function that prints a string
*/

#include "my_printf.h"

int my_putstr(char const *str, int print)
{
    int i = 0;
    int length = 0;

    while (str[length] != '\0') {
        length++;
    }
    while (i < length) {
        my_putchar(str[i], print);
        i++;
    }
    return length;
}
