/*
** EPITECH PROJECT, 2025
** src/utilities/strings/strings_linked_list
** File description:
** Some functions for handling a linked list wiht only a string in it.
** Can be useful
*/

#include "core/minishell.h"
#include "my_printf.h"

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

/**
 * @brief Print the content of the string_t at a index.
 *
 * @param head          The head
 * @param index         The index
 */
void print_string_index(IN string_t *head, IN int index)
{
    string_t *tmp = NULL;
    int i = 0;

    if (head == NULL || index < 0)
        return;
    tmp = head;
    while (tmp != NULL && i < index) {
        tmp = tmp->next;
        i++;
    }
    my_printf("%s", tmp->string);
}

/**
 * @brief Get the content of a string at a index.
 *
 * @param head          The head
 * @param index         The index
 *
 * @return The string not copied but just referenced.
 */
char *get_string_index(IN string_t *head, IN int index)
{
    string_t *tmp = NULL;
    int i = 0;

    if (head == NULL || index < 0)
        return NULL;
    tmp = head;
    while (tmp != NULL && i < index) {
        tmp = tmp->next;
        i++;
    }
    return (tmp != NULL) ? tmp->string : NULL;
}
