/*
** EPITECH PROJECT, 2025
** include/core/minishell
** File description:
** Header file for mini shell project
*/

#ifndef MINISHELL_H
    #define MINISHELL_H

    #include <stdio.h>
    #include <stdbool.h>
    #include <stdlib.h>
    #include <stddef.h>
    #include <stdarg.h>
    #include <unistd.h>
    #include <limits.h>
    #include <signal.h>
    #include <sys/wait.h>
    #include <sys/stat.h>
    #include <sys/ioctl.h>
    #include <errno.h>
    #include <fcntl.h>
    #include <sys/types.h>
    #include <dirent.h>
    #include <termios.h>
    #include <ctype.h>
    #include <string.h>
    #include <time.h>
    #include <regex.h>

    #include "builtins.h"
    #include "macros/math_macros.h"
    #include "macros/misc_macros.h"
    #include "core/types.h"
    #include "core/signals.h"
    #include "core/parser.h"
    #include "my_printf.h"

    #ifndef _DEBUG_MODE_
        #define _DEBUG_MODE_ 0
    #endif /* ifndef _DEBUG_MODE_ */

/*
 * Environment structure used in a linked list.
 * This method is faster than a normal array aka extern char **environ
 * Have directly key and value and the next env var in the same place.
 */
typedef struct env_node_s {
    char *key;
    char *value;
    struct env_node_s *next;
} env_node_t;

/*
 * Variable structure used in a linked list.
 */
typedef struct var_node_s {
    char *_key;
    char **_value;
    int _read_only;
    struct var_node_s *_next;
} var_node_t;

/*
 * Passwd structure
 * This structure contains stuff of a line in /etc/passwd file
 * - user = Name of the user.
 * - pass = Contain info if a password for this user exist or not.
 * - uid, gid = UID and GID of the user.
 * - comment = If there is any comments in the line.
 * - home = Get the home of the user (path).
 * - shell = Get the shell of the user (path).
 */
typedef struct passwd_s {
    char *user;
    char *pass;
    int uid;
    int gid;
    char *comment;
    char *home;
    char *shell;
} passwd_t;

/*
 * Alias structure
 * This structure contains stuff for aliases.
 * - original_string = The original string to be replaced.
 * - alias_string = The final string which will appear for the final output.
 */
typedef struct alias_s {
    char *original_string;
    char *alias_string;
    struct alias_s *next;
} alias_t;

/*
 * Shell variables structure
 * This structure contains all variables used in the shell for exemple HOME,
 * OLDPWD, PWD.
 * We don't use them directly in the *env because if the user delete them
 * you need to make sure you can still use them. So we have this structure
 * to make sure you have the information in any case.
 */
typedef struct shell_variables_s {
    char *home_var;
    char *oldpwd_var;
    char *pwd_var;
    char *path_var;
    char *github_repository;
    int history_lines_count;
} shell_variables_t;

/*
 * Simple string linked list.
 * Use to stock some values for pretty much everything.
 * Used actually for stocking every values to be swapped like $PATH
 */
typedef struct string_s {
    char *string;
    struct string_s *next;
} string_t;

/**
 * @brief Terminal structure informations
 *        Contains stuff for the input line with cursor.
 */
typedef struct term_info_s {
    struct termios _original_termios;
    struct termios _current_termios;
    char _buffer[4096];
    char _yank_buffer[4096];
    int _cursor_start_pos[2];
    int _cursor_pos[2];
    size_t _buffer_len;
    size_t _cursor_index;
    bool _sig_buffer_reset;
    int _history_index;
} term_info_t;

/*
 * Shell structure
 * - env = Linked list with all environment variables.
 * - last_process_status = The last processus exit code. Used with echo $?.
 * - vars = Contains some variables in the shell like HOME, PWD, OLDPWD.
 * - should_exit = If the BUILTIN exit is called then make this true to return.
 * - user_input_buffer = The original input buffer in case you need it.
 */
typedef struct shell_s {
    term_info_t *_term_info;
    string_t *_history_input;
    int _max_history;
    env_node_t *env;
    var_node_t *variables;
    alias_t *aliases;
    shell_variables_t *vars;
    exitcode_t last_exit_code;
    char *last_input_buffer;
    pid_t current_child_pid;
    fd_t saved_stdout;
    fd_t saved_stdin;
    bool is_piped_shell;
    bool should_exit;
} shell_t;

/*
 * Structure to hold globbing process state
 */
typedef struct glob_state {
    char **result;
    int result_index;
    int argc;
    char **argv;
} glob_state_t;

/*
 * Structure for directory processing state
 */
typedef struct dir_process_state {
    char *base_dir;
    char **args;
    int index;
    char **matches;
    int *match_count;
    int *capacity;
} dir_process_state_t;

/*
 * regex_globbing
 */
char **regex_globbing(char **argv);
char *pattern_to_regex(char *pattern);
int has_globbing_chars(char *str);
char *create_path(char *base_dir, char *component);
char **split_path(char *path);
char *get_dir_path(char *path);
int dir_exists(char *path);
int is_valid_directory(char *path);
char *init_regex(char *pattern, regex_t *regex_ptr);
char **init_matches(void);
int should_skip_entry(struct dirent *entry, char *regex_pattern);
void process_with_next(dir_process_state_t *state, char *new_path);
void process_match(dir_process_state_t *state, struct dirent *entry);
char **process_dir_entries(DIR *dir, char *regex_pattern, regex_t *regex,
    dir_process_state_t *state);
void add_to_matches(dir_process_state_t *state, char *new_path);
void add_submatches(dir_process_state_t *state, char **sub_matches);
char **expand_pattern_recursive(char *base_dir, char **components, int index);
char **process_glob_matches(glob_state_t *state, char **matches);
char **handle_no_matches(glob_state_t *state, char **matches, int i);
void sort_paths(char **paths);
char **reallocate_result_array(glob_state_t *state, char **matches,
    int new_size);

/*
 * Autocomplete functions
 */
char **fill_autocomplete(char *path);
int is_visible_file(char *name);
char *my_strcat_alloc(char *str1, char *str2);
int str_in_tab(char **tab, char *str);
int is_executable_file(char *filename);
char **my_str_to_word_array(char *str, char delimiter);

/*
 * Function used in the main
 */
int setup_shell(void);
int shell_loop(void);

/*
 * Shell manager functions
 */
shell_t *get_shell(void);
exitcode_t exit_shell(void);
exitcode_t shell_execute(token_list_t *list_tokens);

/*
 * Shell variables functions
 */
shell_variables_t *create_shell_vars(void);
void print_shells_vars(void);
void free_shell_vars(void);

/*
 * Command handling functions
 */
int show_error_execve(int errno_val, char **argv);
exitcode_t execute_ast_node(ast_node_t *node);
exitcode_t exec_binary(char **argv);
exitcode_t exec_built_in(char **argv);
exitcode_t exec_built_in(char **argv);
exitcode_t execute_command_fork(ast_node_t *ast);
int execute_pipes(ast_node_t *node);
int execute_redirection(ast_node_t *node);
int execute_and(ast_node_t *node);
int execute_or(ast_node_t *node);
exitcode_t get_wait_status(int wait_pid_result);

/*
 * Globbing handling functions
 */
char **change_star_to_list_of_files(char **argv);
char **filter_array(char **array, char *pattern);
char **read_dir_entries(DIR *dir, int count);
int count_files(char *path);
int index_of_last_occurrence(char *str, char c);
int count_matches(char **array, char *pattern);
void fill_matches(char **array, char *pattern, char **result);
void copy_before_globbing(char **argv, char **result, int globbing_index);
void copy_matches(char **matches, char **result, int globbing_index);
void copy_after_globbing(char **argv, char **result, int start_pos,
    int result_index);
char **process_globbing_pattern(char **argv, int globbing_index);
char *create_path_to_dir(char *globbing_string);
int starts_with(char *str, char *prefix, int prefix_len);
int ends_with(char *str, char *suffix, int str_len, int suffix_len);
char **replace_globbing_with_matches(char **argv, int globbing_index,
    char **matches);
char *remove_dot_slash(char *path);

/*
 * Environment handling functions
 */
void add_env(char *key, char *value);
int remove_env(char *key);
char *env_search(char *key);
void clear_env(void);
void reset_initial_env(void);

/*
 * Local variables
 */
void free_var_value(var_node_t *var);
void clear_var(void);
int is_var_readonly(var_node_t *var);
int remove_var(char *key);
char **var_search(char *key);
char *concat_strarray(char **array, char *separator);
void insert_alphabetically(var_node_t *var, var_node_t *new_var,
    char *key);

/*
 * Path variable
*/
void add_path_variable(shell_variables_t *vars);
void update_env_path(char *key, char **value);
void update_var_path(void);

/*
 * Termios helping functions
 */
term_info_t *setup_shell_term_info(void);
void init_termios(void);
char *termios_get_input(void);
void reset_buffer_termios(term_info_t *term_info);
void enable_raw_mode(shell_t *shell);
void get_cursor_position(int *row, int *col);
void print_multiline_buffer(term_info_t *ti);
size_t get_lines_amount_buffer(term_info_t *ti);
void set_cursor_position(int y, int x);
void move_cursor_from_position(int move, term_info_t *ti);
void setup_new_prompt(term_info_t *ti);

struct winsize get_screen_info(void);
void print_remaining_stdin(void);
bool has_remaining_input(void);

void handle_ctrl_e(term_info_t *ti);
void handle_ctrl_a(term_info_t *ti);
void handle_ctrl_k(term_info_t *ti);
void handle_ctrl_y(term_info_t *ti);
void handle_autocomplete(term_info_t *ti);
void modify_buffer_suggestion(term_info_t *ti, char *current_sugg,
    char *curr_word);
void modify_buffer_single(term_info_t *ti, char *current_sugg);

void handle_character(term_info_t *ti, char c);
void handle_backspace(term_info_t *ti);
void handle_left_arrow(term_info_t *ti);
void handle_right_arrow(term_info_t *ti);
void handle_history_up(term_info_t *ti);
void handle_history_down(term_info_t *ti);

/*
 * Environment transformer functions
 */
char **env_node_to_array(env_node_t *env);

/*
 * Binary paths functions
 */
char *get_binary_path(char *command);

/*
 * String utils functions
 */
int my_strcmp(char *s1, char *s2);
int my_strncmp(char *s1, char *s2, size_t n);
char *my_strcpy(char *dest, char const *src);
char *my_strncpy(char *dest, char const *src, size_t n);
char *my_strncpy_alloc(char const *src, size_t n);
char *my_strdup(char const *src);
char *my_strtok(char *string, char until_char);
char *my_strtok_reverse(char *string, char until_char);
char **my_strarray(char const *src);
char *my_strcat(char *dest, char const *src);
char *my_strncat(char *dest, char const *src, size_t n);
char *my_strreplace(char *string, char *to_replace, char *replace_string);
char **my_strreplace_array(char **array, char *to_replace, char *replace_str);
char *my_strreplace_full(char *string, char *from, char *to);
char **my_strreplace_array_full(char **array, char *to_replace, char *replace);
char *my_strcat_list(char *dest, ...);
int is_input_delimiter(char c, int i, const char *str);
int is_quote_delimiter(char c, int i, const char *str);
string_t *add_string(string_t *head, char *string);
void free_strings(string_t *head);
void print_strings(string_t *head);
void print_string_index(string_t *head, int index);
char *get_string_index(string_t *head, int index);
bool is_in_string(char c, char *string);

void remove_newline(char *str);
char **command_formatter(char **argv);
void format_ast(ast_node_t *ast);

int count_backslash(char *string, int i);
char **handle_inhibitors_array(char **array);
char *handle_inhibitors(char *string);
char *handle_quotes(char *string);
char **handle_quotes_array(char **array);

int print_err(char *message, ...);

bool is_letter(char c);
bool is_only_numbers(char *string);
bool is_alpha_num(char *string);
int len_to_first_char(char *str);

int char_in_str(char *str, char c);
int find_char_index_in_tab(char **tab, char c);

/*
 * Files functions
 */
bool is_file_type(char *path_file, mode_t type);
bool is_file_exist(char *path_file);

/*
 * Prompt helper functions
 */
void print_shell_prompt(void);

/*
 * Aliases handler functions
 */
void add_alias(char *original_string, char **alias_arr);
int remove_alias(char *original_string);
void clear_aliases(void);
char **replaces_all_aliases(char **input);

/*
 * Passwd helper functions
 */
passwd_t *get_passwd_from_user(char *username);
void print_passwd(passwd_t *passwd);
void free_passwd(passwd_t *passwd);

/*
 * Utils functions
 */
void print_array(char **array);
void free_array_string(char **array);

/*
 * Mathematics functions
 */
long my_powl(long number, int exponent);

/*
 * Parsing functions
 */
int my_atoi(char *string);
char *my_itoa(int number);
size_t read_line(int fd, char *buffer, size_t max_len);

/*
 * Array functions
 */
size_t array_count_string(char **string_array);
char **array_add_index_string(char **original, char *to_add, size_t index);
char **array_append_index_string(char **original, char **append, size_t index);
char **array_remove_index_string(char **original, size_t index);

/*
 * Modified system calls
 */
void free_null_check(void *pointer);

/*
 * myshrc functions
 */
int load_myshrc(void);

/*
 * my_sh_history functions
 */
int open_history_file(void);
int write_command_history(char *command);
int count_number_lines_history(void);
char *get_sh_history_path(void);

/*
 * Environments variables extracting formatting functions
 */
string_t *extract_vars_in_string(string_t *strings, char *input);
string_t *extract_vars_in_array(char **array);
int replace_env_variables(char ***argv);
int replace_variables(char ***argv);

/*
 * Github features functions
 */
bool is_in_github_repository(void);
char *get_github_repository_name(void);

#endif /* ifndef MINISHELL_H */
