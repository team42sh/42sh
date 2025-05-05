/*
** EPITECH PROJECT, 2025
** src/strings/my_strarray
** File description:
** This file contains implementation of a str to word array
*/

#include "core/minishell.h"
#include "my_printf.h"

/*
 * Intermediate function for counting the words final array.
 * String is the string to get the number of words.
 * In quotes is a reference for the variable to know if the function is
 * currently in a quotes or not.
 * Amount is the reference to increment when in a new word.
 * Index i is the index to know if it's a new word.
 */
static void is_counted_as_new_word(IN char const *string, OUT bool *in_quotes,
    OUT int *amount, IN int *i)
{
    if (is_quote_delimiter(string[*i], *i > 0 ? string[*i - 1] : 0)) {
        *in_quotes = !(*in_quotes);
        if (*in_quotes && is_input_delimiter(*i > 0 ? string[*i - 1] : ' ',
            *i > 1 ? string[*i - 2] : 0))
            (*amount)++;
        return;
    }
    if (!(*in_quotes) && !is_input_delimiter(string[*i],
        *i > 0 ? string[*i - 1] : 0) &&
        (*i == 0 || is_input_delimiter(string[*i - 1],
            *i > 1 ? string[*i - 2] : 0)))
        (*amount)++;
}

/*
 * Count the number of words in the string.
 * This function handle the quote delimiter.
 * Used to allocate the words final array.
 */
static int count_words(IN char const *string)
{
    bool in_quotes = 0;
    int amount = 0;
    int len;

    if (!string)
        return 0;
    len = my_strlen(string);
    if (len > MAX_WORD_LEN)
        len = MAX_WORD_LEN;
    for (int i = 0; i < len && amount < MAX_WORD_LEN; i++)
        is_counted_as_new_word(string, &in_quotes, &amount, &i);
    return amount;
}

/*
 * Get the length of a word in the string.
 * Handle 2 cases :
 * - "word word"
 * - word
 */
static int get_word_length(IN const char *str)
{
    int len = 0;
    int in_quote = 0;

    if (!str)
        return 0;
    while (str[len] && (!is_input_delimiter(str[len],
        len > 0 ? str[len - 1] : 0) || in_quote)
    && len < MAX_WORD_LEN) {
        if (str[len] == '"')
            in_quote = !in_quote;
        if (str[len])
            len++;
    }
    return len;
}

/*
 * Extract the word of len <len> in the source <src>.
 * Copy it into a allocated string and return it with \0.
 */
static char *extract_word(IN char const *src, IN int len)
{
    char *word = malloc(sizeof(char) * (len + 1));

    if (word == NULL)
        return NULL;
    my_strncpy(word, src, len);
    word[len] = '\0';
    return word;
}

/*
 * Skip the delimiters put in the function is_input_delimiter.
 * And return the final index.
 */
static int skip_delimiters(IN const char *str, IN int index)
{
    while (str[index] && is_input_delimiter(str[index],
        index > 0 ? str[index - 1] : 0))
        index++;
    return index;
}

/*
 * Do the main logic for the str_array.
 * Populate the words list by extracting every words until a delimiter.
 */
static char **populate_word_array(IN char const *str,
    OUT char **words, IN int word_len)
{
    int word_i = 0;
    int temp_word_len = 0;

    for (int i = 0; str[i] != '\0' && word_i < word_len;) {
        i = skip_delimiters(str, i);
        if (!str[i])
            break;
        temp_word_len = get_word_length(str + i);
        if (temp_word_len <= 0 || temp_word_len > MAX_WORD_LEN)
            return NULL;
        words[word_i] = extract_word(str + i, temp_word_len);
        if (!words[word_i])
            return NULL;
        word_i++;
        i += temp_word_len;
    }
    words[word_i] = NULL;
    return words;
}

/*
 * Is a delimiter ?
 * Delimiter are basically :
 * - ' '
 * - '\t'
 */
int is_input_delimiter(IN char c, IN char prev_c)
{
    return ((c == ' ' || c == '\t') && prev_c != '\\');
}

/*
 * Verify if the current char is a quote delimiter, that mean opening or
 * closing a quote.
 * This need to be placed without a \ in the previous character.
 */
int is_quote_delimiter(IN char c, IN char prev_c)
{
    return (c == '"' && prev_c != '\\');
}

/*
 * Split a string into an array of words. Handles quoted words as a
 * single ENTITY.
 * The resulting array is NULL terminated.
 * TODO: Handle ' with "
 */
char **my_strarray(IN char const *str)
{
    int word_count = 0;
    char **words = NULL;

    if (str == NULL)
        return NULL;
    word_count = count_words(str);
    if (word_count <= 0 || word_count > MAX_WORDS_COUNT)
        return NULL;
    words = malloc(sizeof(char *) * (word_count + 1));
    if (words == NULL)
        return NULL;
    if (!populate_word_array(str, words, word_count)) {
        free_array_string(words);
        return NULL;
    }
    return words;
}

/*
 * Free the content of a char **array.
 * This array need to be NULL terminated.
 */
void free_array_string(IN char **array)
{
    if (array == NULL)
        return;
    for (size_t i = 0; array[i] != NULL; i++) {
        free_null_check(array[i]);
        array[i] = NULL;
    }
    free(array);
    array = NULL;
}
