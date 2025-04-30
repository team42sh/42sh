/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** history
*/

#include "core/minishell.h"
#include "core/builtins.h"
#include "core/builtins/history.h"

/**
 * @brief Resize the lines array to a new size.
 *
 * @param lines The array of lines to resize.
 * @param new_size The new size of the array.
 * @return char** The resized array of lines.
 * @note The caller is responsible for freeing the old array.
 */
static char **resize_lines_array(IN char **lines, IN size_t new_size)
{
    return realloc(lines, new_size * sizeof(char *));
}

/**
 * @brief Store a line in the lines array without a limit on the lines.
 *
 * @param lines The array of lines to store the line in.
 * @param stored The number of lines currently stored in the array.
 * @param line The line to store.
 * @return int 0 on success, -1 on failure.
 */
static int store_line_unlimited(OUT char ***lines, OUT size_t *stored,
    IN char *line)
{
    char **tmp = resize_lines_array(*lines, *stored + 1);

    if (!tmp)
        return -1;
    *lines = tmp;
    (*lines)[*stored] = strdup(line);
    if (!(*lines)[*stored])
        return -1;
    (*stored)++;
    return 0;
}

/**
 * @brief Store a line in the lines array with a limit on the number of lines.
 *
 * @param lines The array of lines to store the line in.
 * @param stored The number of lines currently stored in the array.
 * @param max The maximum number of lines to keep.
 * @param line The line to store.
 * @return int 0 on success, -1 on failure.
 */
static int store_line_limited(IN char **lines, IN size_t *stored,
    IN int max, IN char *line)
{
    free(lines[*stored % max]);
    lines[*stored % max] = strdup(line);
    if (!lines[*stored % max])
        return -1;
    (*stored)++;
    return 0;
}

/**
 * @brief Free all lines in the array and the array itself.
 *
 * @param lines The array of lines to free.
 * @param stored The number of lines currently stored in the array.
 * @param limit The maximum number of lines to keep (-1 for unlimited).
 */
void free_all_lines(IN char **lines, IN size_t stored, IN int limit)
{
    size_t max = (limit == -1 || stored < (size_t)limit)
        ? stored : (size_t)limit;

    if (!lines)
        return;
    for (size_t i = 0; i < max; i++)
        free(lines[i]);
    free(lines);
}

/**
 * @brief Finalize the lines array by limiting the number of lines.
 *
 * @param lines The array of lines to finalize.
 * @param stored The number of lines currently stored in the array.
 * @param limit The maximum number of lines to keep (-1 for unlimited).
 * @return char** An array of strings containing the finalized lines.
 */
static char **finalize_lines_array(IN char **lines, IN size_t stored,
    IN int limit)
{
    size_t start = 0;
    size_t count = stored;
    char **result = NULL;

    if (limit > 0 && stored > (size_t)limit) {
        start = stored - limit;
        count = limit;
    }
    result = calloc(count + 1, sizeof(char *));
    if (!result)
        return NULL;
    for (size_t i = 0; i < count; i++) {
        result[i] = strdup(lines[(start + i) % ((limit > 0)
            ? (size_t)limit : (size_t)stored)]);
    }
    result[count] = NULL;
    return result;
}

/**
 * @brief Read lines from a file and store them in an array.
 *
 * @param file The file to read from.
 * @param lines The array of lines to store the read lines.
 * @param stored The number of lines currently stored in the array.
 * @param lines_count The maximum number of lines to read (-1 for unlimited).
 * @return int 0 on success, -1 on failure.
 */
static int read_lines(IN FILE *file, OUT char ***lines, OUT size_t *stored,
    IN int lines_count)
{
    char *line = NULL;
    size_t len = 0;
    int err = 0;

    while (getline(&line, &len, file) != -1) {
        err = (lines_count == -1)
            ? store_line_unlimited(lines, stored, line)
            : store_line_limited(*lines, stored, lines_count, line);
        if (err) {
            free(line);
            return -1;
        }
    }
    free(line);
    return 0;
}

/**
 * @brief Close the file and returns null.
 *
 * @param file The file to close.
 * @return char** An array of strs containing the lines from the file (NULL).
 */
static void *close_and_return_null(IN FILE *file)
{
    fclose(file);
    return NULL;
}

/**
 * @brief Parse the history file and return an array of lines.
 *
 * @param file_path The path to the history file.
 * @param lines_count The number of lines to read from the file.
 * @return char** An array of strings containing the lines from the file.
 */
char **parse_history_file(IN char *file_path, IN int lines_count)
{
    FILE *file = fopen(file_path, "r");
    size_t stored = 0;
    char **lines = NULL;
    char **result = NULL;

    if (!file)
        return NULL;
    lines = calloc(lines_count > 0 ? lines_count : 1, sizeof(char *));
    if (!lines)
        return close_and_return_null(file);
    if (read_lines(file, &lines, &stored, lines_count) != 0) {
        free_all_lines(lines, stored, lines_count);
        return close_and_return_null(file);
    }
    fclose(file);
    result = finalize_lines_array(lines, stored, lines_count);
    free_all_lines(lines, stored, lines_count);
    return result;
}
