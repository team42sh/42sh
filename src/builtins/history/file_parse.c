/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** history
*/

#include "core/minishell.h"
#include "core/builtins.h"
#include "core/builtins/history.h"

static char **resize_lines_array(char **lines, size_t new_size)
{
    return realloc(lines, new_size * sizeof(char *));
}

static int store_line_unlimited(char ***lines, size_t *stored, char *line)
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

static int store_line_limited(char **lines, size_t *stored,
    int max, char *line)
{
    free(lines[*stored % max]);
    lines[*stored % max] = strdup(line);
    if (!lines[*stored % max])
        return -1;
    (*stored)++;
    return 0;
}

void free_all_lines(char **lines, size_t stored, int limit)
{
    size_t max = (limit == -1 || stored < (size_t)limit)
        ? stored : (size_t)limit;

    if (!lines)
        return;
    for (size_t i = 0; i < max; i++)
        free(lines[i]);
    free(lines);
}

static char **finalize_lines_array(char **lines, size_t stored,
    int limit)
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

static int read_lines(FILE *file, char ***lines, size_t *stored,
    int lines_count)
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

char **parse_history_file(IN char *file_path, int lines_count)
{
    FILE *file = fopen(file_path, "r");
    size_t stored = 0;
    char **lines = NULL;
    char **result = NULL;

    if (!file)
        return NULL;
    lines = calloc(lines_count > 0 ? lines_count : 1, sizeof(char *));
    if (!lines) {
        fclose(file);
        return NULL;
    }
    if (read_lines(file, &lines, &stored, lines_count) != 0) {
        fclose(file);
        free_all_lines(lines, stored, lines_count);
        return NULL;
    }
    fclose(file);
    result = finalize_lines_array(lines, stored, lines_count);
    free_all_lines(lines, stored, lines_count);
    return result;
}
