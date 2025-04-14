/*
** EPITECH PROJECT, 2025
** src/utilities/strings/strings_linked_list
** File description:
** Some functions for handling a linked list wiht only a string in it.
** Can be useful
*/

#include "core/minishell.h"

/*
 * Add a string in the linked list called head.
 */
string_t *add_string(string_t *head, char *string)
{
    string_t *node;

    if (string == NULL)
        return NULL;
    node = malloc(sizeof(string_t));
    if (node == NULL)
        return NULL;
    node->string = string;
    node->next = head;
    return node;
}

/*
 * Free the entire linked list strings.
 */
void free_strings(string_t *head)
{
    string_t *tmp = NULL;

    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp->string);
        free(tmp);
    }
}

/*
 * Print the content of the linked list.
 */
void print_strings(string_t *head)
{
    string_t *tmp = head;

    while (tmp != NULL) {
        my_printf("%s\n", tmp->string);
        tmp = tmp->next;
    }
}
