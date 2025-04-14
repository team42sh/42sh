/*
** EPITECH PROJECT, 2024
** my_strlen
** File description:
** A function that returns the length of a string
*/

#include <unistd.h>

int my_strlen(char const *str)
{
    int i = 0;

    if (str == NULL)
        return 0;
    while (str[i] != '\0') {
        i++;
    }
    return i;
}
