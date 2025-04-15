/*
** EPITECH PROJECT, 2025
** src/formatting/globbings/everything_globbing
** File description:
** File to handle the everythinng globbing
*/

#include "core/minishell.h"
#include "core/parser.h"
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

int count_files(IN char *path)
{
    DIR *dir;
    int i = 0;

    if (path == NULL)
        return -1;
    dir = opendir(path);
    while (readdir(dir) != NULL)
        i++;
    return (i > 0) ? i : -1;
}

int char_in_str(IN char *str, IN char c)
{
    if (str == NULL || c == '\0')
        return -1;
    for (int i = 0; str[i] != '\0'; i++)
        if (str[i] == c)
            return i;
    return -1;
}

int find_char_index_in_tab(IN char **tab, IN char c)
{
    if (tab == NULL || c == '\0')
        return -1;
    for (int i = 0; tab[i] != NULL; i++)
        if (char_in_str(tab[i], c) != -1)
            return i;
    return -1;
}

char *create_path_to_dir(IN char *globbing_string)
{
    char *result_path = NULL;
    int globbing_index = char_in_str(globbing_string, '*');

    if (my_strcmp(globbing_string, "*") == 0) {
        result_path = my_strdup(".");
        return result_path;
    }
    if (globbing_string[globbing_index - 1] != '\0' &&
        globbing_string[globbing_index - 1] == '/') {
        result_path = malloc(sizeof(char) * (globbing_index));
    }
    return NULL;
}

char **change_star_to_list_of_files(IN char **argv)
{
    int globbing_index = find_char_index_in_tab(argv, '*');
    char *path = NULL;

    if (globbing_index == -1)
        return argv;
    path = create_path_to_dir(argv[globbing_index]);
    free(path);
    return NULL;
}
