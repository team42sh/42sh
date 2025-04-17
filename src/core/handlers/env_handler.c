/*
** EPITECH PROJECT, 2025
** src/env/get_env
** File description:
** This file will contains function for getting any environment variable
*/

#include "core/minishell.h"

/*
 * Create a node using key, value for the environment variable.
 * Return the structure of the node.
 */
static env_node_t *create_node(char *key, char *value)
{
    env_node_t *node = malloc(sizeof(env_node_t));

    if (node == NULL)
        return NULL;
    node->key = key;
    node->value = value;
    node->next = NULL;
    return node;
}

/*
 * Free a node of the environment variables.
 */
static void free_env(env_node_t *env)
{
    if (env == NULL)
        return;
    free_null_check(env->key);
    free_null_check(env->value);
    free_null_check(env);
}

/*
 * Get an environment variable value from a key, will search it on the array
 * in the shell structure.
 * Return the reference of the value. Not a COPY!
 */
char *env_search(char *key)
{
    env_node_t *env = get_shell()->env;

    if (key == NULL)
        return NULL;
    while (env != NULL) {
        if (my_strcmp(env->key, key) == 0)
            return env->value;
        env = env->next;
    }
    return NULL;
}

/*
 * Add a key and value into the environment variable in the shell structure.
 * This function add it on the top of the linked list.
 */
void add_env(char *key, char *value)
{
    env_node_t *env = get_shell()->env;
    env_node_t *tmp = NULL;

    while (env != NULL) {
        if (my_strcmp(env->key, key) == 0) {
            free(env->value);
            env->value = value;
            return;
        }
        env = env->next;
    }
    env = get_shell()->env;
    if (env == NULL) {
        get_shell()->env = create_node(key, value);
        return;
    }
    tmp = create_node(key, value);
    while (env->next != NULL)
        env = env->next;
    env->next = tmp;
}

/**
 * @brief A function to delete the first element of a linked list
 *
 * @param key
 * @param env
 * @param before
 * @return int
 */
static int remove_if_first(IN char *key, OUT env_node_t **env,
    OUT env_node_t **before)
{
    if (*env == NULL)
        return OK_OUTPUT;
    if (*env != NULL && my_strcmp((*env)->key, key) == 0) {
        get_shell()->env = (*env)->next;
        free_env(*env);
        return 0;
    }
    *before = *env;
    *env = (*env)->next;
    return COMMAND_ERROR;
}

/*
 * Remove an environment variable using the key.
 * Return -1 if any error occur. 0 if success.
 */
int remove_env(char *key)
{
    env_node_t *env = get_shell()->env;
    env_node_t *before = NULL;
    int remove_first_out;

    remove_first_out = remove_if_first(key, &env, &before);
    if (remove_first_out != COMMAND_ERROR)
        return remove_first_out;
    while (env != NULL) {
        if (my_strcmp(key, env->key) == 0) {
            before->next = env->next;
            free_env(env);
            return 0;
        }
        before = env;
        env = env->next;
    }
    return -1;
}

/*
 * This function will remove all environment variables in the shell.
 * Can be dangerous.
 */
void clear_env(void)
{
    env_node_t *next_tmp = NULL;

    for (env_node_t *env = get_shell()->env; env != NULL; env = next_tmp) {
        next_tmp = env->next;
        free_env(env);
    }
    get_shell()->env = NULL;
}

/*
 * This function will reset the structure of the sell for the environment
 * variables.
 * Remove all environment then add all the default ones. Using the environ var.
 */
void reset_initial_env(void)
{
    extern char **environ;
    char *tmp_key = NULL;
    char *tmp_value = NULL;

    clear_env();
    if (environ == NULL)
        return;
    for (int i = 0; environ[i] != NULL; i++) {
        tmp_key = my_strtok(environ[i], '=');
        tmp_value = my_strdup(&(environ[i][my_strlen(tmp_key) + 1]));
        if (tmp_key == NULL || tmp_value == NULL)
            continue;
        add_env(tmp_key, tmp_value);
    }
}
