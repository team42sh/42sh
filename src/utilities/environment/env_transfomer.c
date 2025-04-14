/*
** EPITECH PROJECT, 2025
** src/utilities/environment/env_transfomer
** File description:
** This file contains functions for doing this manipulation :
** Linked list (env) to char **env
*/

#include "../../../include/core/minishell.h"

/*
 * This function count the number of environment variables in the linked list.
 * This is principaly used for allocating the right numbers of variables in
 * the array.
 */
static int count_env(env_node_t *env)
{
    env_node_t *tmp = env;
    size_t count = 0;

    while (tmp != NULL) {
        count++;
        tmp = tmp->next;
    }
    return count;
}

/*
 * Will create a new allocated char ** which will contains stuff of the
 * linked list.
 * Create a array from a linked list basically with 'key=value'
 */
char **env_node_to_array(env_node_t *env)
{
    size_t len_env = count_env(env);
    size_t temp_len = 0;
    char **env_array = malloc(sizeof(char *) * (len_env + 1));
    size_t i = 0;

    for (env_node_t *tmp = env; tmp != NULL; tmp = tmp->next) {
        temp_len = my_strlen(tmp->value) + my_strlen(tmp->key) + 1;
        env_array[i] = malloc(sizeof(char) * (temp_len + 1));
        env_array[i][0] = '\0';
        my_strcat(env_array[i], tmp->key);
        my_strcat(env_array[i], "=");
        my_strcat(env_array[i], tmp->value);
        env_array[i][temp_len] = '\0';
        i++;
    }
    env_array[len_env] = NULL;
    return env_array;
}
