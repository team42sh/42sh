/*
** EPITECH PROJECT, 2025
** src/utilities/parsing/readline
** File description:
** This function can read line
*/

#include "../../../include/core/minishell.h"

/*
 * Read the fd until a \n, and conserve it.
 * Looks like a getline but with doesn't allocate automatically.
 * Need static buffer.
 */
size_t read_line(int fd, char *buffer, size_t max_len)
{
    size_t bytes_read = 0;
    size_t i = 0;
    char c;

    while (i < max_len - 1) {
        bytes_read = read(fd, &c, 1);
        if (bytes_read <= 0)
            break;
        buffer[i] = c;
        i++;
        if (c == '\n')
            break;
    }
    buffer[i] = '\0';
    return i;
}
