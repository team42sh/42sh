/*
** EPITECH PROJECT, 2025
** src/utilities/parsing/itoa
** File description:
** Reverse of ATOI function. Integer to string.
*/

#include "../../../include/core/minishell.h"

/*
 * Get the number of digit of the number.
 * Length is 1 if it's 0 as number since the code will not go through
 * the while.
 */
static int get_digits(int number)
{
    int temp = number;
    int length = (number == 0 ? 1 : 0);

    while (temp != 0) {
        temp /= 10;
        length++;
    }
    return length;
}

/*
 * Inverse function of ATOI. Give a number capped at INT_MAX and return his
 * string.
 * This function handle the negative and return a allocated string.
 */
char *my_itoa(int number)
{
    int length = get_digits(number);
    int is_negative = number < 0 ? 1 : 0;
    char *result;

    length += is_negative;
    result = malloc(sizeof(char) * (length + 1));
    if (result == NULL)
        return NULL;
    result[length] = '\0';
    for (int i = length - 1; i >= is_negative; i--) {
        result[i] = (ABS(number) % 10) + '0';
        number /= 10;
    }
    if (is_negative)
        result[0] = '-';
    else if (length == 1 && result[0] == '\0')
        result[0] = '0';
    return result;
}
