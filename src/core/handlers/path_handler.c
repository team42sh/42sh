/*
** EPITECH PROJECT, 2025
** path_handler
** File description:
** Functions to manipulate the path in the 42sh
*/

#include "core/minishell.h"

static void copy_path_to_var(char *path_env, var_node_t *new_var)
{
    char *path_elem;
    int path_count = 0;

    if (path_env == NULL)
        return;
    new_var->_value = calloc(1, sizeof(char *));
    path_elem = my_strtok(path_env, ':');
    while (path_elem != NULL) {
        if (path_elem == NULL)
            break;
        new_var->_value = realloc(new_var->_value,
            sizeof(char *) * (path_count + 2));
        new_var->_value[path_count] = my_strdup(path_elem);
        new_var->_value[path_count + 1] = NULL;
        path_count++;
        free_null_check(path_elem);
        path_elem = my_strtok(NULL, ':');
    }
    free_null_check(path_elem);
}

void add_path_variable(shell_variables_t *vars)
{
    var_node_t *new_var = calloc(1, sizeof(var_node_t));
    char *path_env = vars->path_var;

    if (new_var == NULL)
        return;
    new_var->_key = my_strdup("path");
    copy_path_to_var(path_env, new_var);
    insert_alphabetically(get_shell()->variables, new_var, "path");
}

void update_env_path(char *key, char **value)
{
    if (my_strcmp(key, "path") != 0)
        return;
    free_null_check(get_shell()->vars->path_var);
    get_shell()->vars->path_var = concat_strarray(value, ":");
    add_env("PATH", my_strdup(get_shell()->vars->path_var));
    return;
}

static void separate_var_path(char *var_path)
{
    for (int i = 0; var_path[i] != '\0'; i++) {
        if (var_path[i] == ':') {
            var_path[i] = ' ';
            continue;
        }
    }
}

void update_var_path(void)
{
    char *var_path = get_shell()->vars->path_var;
    char **argv = calloc(3, sizeof(char *));

    argv[0] = my_strdup("set");
    argv[1] = calloc(my_strlen(var_path) + 5, sizeof(char *));
    my_strcpy(argv[1], "path=");
    separate_var_path(var_path);
    my_strcat(argv[1], var_path);
    set_command(argv);
    free_null_check(argv[0]);
    free_null_check(argv[1]);
    free_null_check(argv);
}
