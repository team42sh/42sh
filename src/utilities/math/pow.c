/*
** EPITECH PROJECT, 2025
** src/utils/my_pow
** File description:
** This file contains function for pow functions
*/

#include "core/minishell.h"

/*
 * Base function for power function using long type.
 */
long my_powl(long number, int exponent)
{
    long result = 1;
    int current_exp = 0;

    while (current_exp < exponent) {
        result *= number;
        current_exp++;
    }
    return result;
}
