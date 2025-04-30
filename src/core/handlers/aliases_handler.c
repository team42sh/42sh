/*
** EPITECH PROJECT, 2025
** src/core/handlers/aliases_handler
** File description:
** This file contains some functions for handling aliases
*/

#include "core/minishell.h"
#include "my_printf.h"

/*
 * Create a node using original string, alias string for the alias.
 * Return the structure of the node.
 */
static alias_t *create_node(char *original, char *alias)
{
    alias_t *alias_node = malloc(sizeof(alias_t));

    if (original == NULL || alias == NULL)
        return NULL;
    alias_node->alias_string = alias;
    alias_node->original_string = my_strdup(original);
    alias_node->next = NULL;
    return alias_node;
}

/*
 * Free a node of the alias.
 */
static int free_alias(alias_t *alias)
{
    if (alias == NULL)
        return -1;
    free_null_check(alias->original_string);
    free_null_check(alias->alias_string);
    free(alias);
    return 0;
}

/**
 * @brief By passing a char ** make 1 char *
 *
 * @param alias_arr             The array of strings
 *
 * @return The alias string.
 */
static char *make_alias_one_string(IN char **alias_arr)
{
    size_t total_len = 0;
    char *result = NULL;

    if (!alias_arr)
        return NULL;
    for (int i = 0; alias_arr[i]; i++) {
        total_len += my_strlen(alias_arr[i]);
        if (alias_arr[i + 1])
            total_len += 1;
    }
    result = malloc(total_len + 1);
    result[0] = '\0';
    for (int i = 0; alias_arr[i]; i++) {
        my_strcat(result, alias_arr[i]);
        if (alias_arr[i + 1] != NULL)
            my_strcat(result, " ");
    }
    return result;
}

/*
 * Add an aliase at the top of the linked list aliases in the shell structure.
 * If the alias is already existing then just modifying it.
 */
void add_alias(char *original_string, char **alias_arr)
{
    alias_t *aliases = get_shell()->aliases;
    char *alias_string = make_alias_one_string(alias_arr);

    while (aliases != NULL) {
        if (my_strcmp(aliases->original_string, original_string) == 0) {
            free(aliases->alias_string);
            aliases->alias_string = alias_string;
            return;
        }
        aliases = aliases->next;
    }
    if (get_shell()->aliases == NULL) {
        get_shell()->aliases = create_node(original_string, alias_string);
        return;
    }
    aliases = get_shell()->aliases;
    while (aliases->next != NULL)
        aliases = aliases->next;
    aliases->next = create_node(original_string, alias_string);
}

/*
 * Remove an alias using the original string which is basically a key.
 * Return -1 if any error occur. 0 if success.
 */
int remove_alias(char *original_string)
{
    alias_t *aliases = get_shell()->aliases;
    alias_t *before = NULL;

    if (aliases == NULL)
        return -1;
    if (aliases != NULL
        && my_strcmp(aliases->original_string, original_string) == 0) {
        get_shell()->aliases = aliases->next;
        return free_alias(aliases);
    }
    before = aliases;
    aliases = aliases->next;
    for (; aliases != NULL; aliases = aliases->next) {
        if (my_strcmp(original_string, aliases->original_string) == 0) {
            before->next = aliases->next;
            return free_alias(aliases);
        }
        before = aliases;
    }
    return -1;
}

/*
 * This function will remove all aliases in the shell.
 * Can be dangerous.
 */
void clear_aliases(void)
{
    alias_t *next_tmp = NULL;

    for (alias_t *al = get_shell()->aliases; al != NULL; al = next_tmp) {
        next_tmp = al->next;
        free_alias(al);
    }
    get_shell()->aliases = NULL;
}
