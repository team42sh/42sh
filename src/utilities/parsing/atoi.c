/*
** EPITECH PROJECT, 2025
** src/utils/my_atoi
** File description:
** Implementation of atoi function
*/

#include "core/minishell.h"

/*
 * Will count the number of '-' in the string.
 * If the total of minus is % 2 == 1 then it's negative.
 */
static int is_negative(char *string)
{
    int counter_minus = 0;

    for (int i = 0; string[i] != '\0' && !IS_NUMBER(string[i]); i++) {
        if (string[i] == '-')
            counter_minus++;
    }
    return counter_minus % 2;
}

/*
 * By counting the number of character until a number then we know where the
 * int start in the string.
 */
static int get_number_start_index(char *string)
{
    int index = 0;

    while (string[index] != '\0' &&
        (string[index] == ' '
        || string[index] == '\t'
        || string[index] == '+'
        || string[index] == '-'))
        index++;
    return index;
}

/*
 * This function will return the len of a number started in [0].
 * Example : "42ba" --> 2
 */
static int get_number_len(char *string)
{
    int index = 0;

    while (string[index] != '\0' && IS_NUMBER(string[index]))
        index++;
    return index;
}

/*
 * Start adding each digit with his right power, to get the final number.
 * This function need to be called when the string[0] is a number already.
 * Don't use this function if the string start without a number.
 */
static int get_number(char *string, int is_negative)
{
    int number_len = get_number_len(string);
    long long result = 0;

    for (int i = 0; i < number_len; i++)
        result += (string[i] - 48) * my_powl(10, number_len - i - 1);
    if (is_negative == 1)
        result *= -1;
    if (result > INT_MAX)
        return INT_MAX;
    if (result < INT_MIN)
        return INT_MIN;
    return result;
}

/*
 * This function is the re-implementation of atoi function in lib C.
 * This function can count and automatically put the number in negate if there
 * is minus in the string.
 * Can handled this example : "-------42" as well.
 */
int my_atoi(char *string)
{
    int negate;
    int number;

    if (string == NULL)
        return 0;
    negate = is_negative(string);
    number = get_number(&string[get_number_start_index(string)], negate);
    return number;
}
