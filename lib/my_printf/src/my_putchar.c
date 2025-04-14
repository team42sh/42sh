/*
** EPITECH PROJECT, 2024
** my_putchar
** File description:
** A function that displays a single character
*/

#include <unistd.h>
#include "my_printf.h"

int my_putchar(char c, int print)
{
    if (print)
        write(1, &c, 1);
    return 1;
}
