/*
** EPITECH PROJECT, 2025
** src/core/helpers/passwd_helper
** File description:
** This file contains function for managing with the passwd file
*/

#include "../../../include/core/minishell.h"

/*
 * Init a structure called passwd which contains every informations of a user.
 * Read the file /etc/passwd to get the informations.
 */
static passwd_t *init_passwd(void)
{
    passwd_t *passwd = malloc(sizeof(passwd_t));

    passwd->uid = -1;
    passwd->gid = -1;
    passwd->home = NULL;
    passwd->pass = NULL;
    passwd->user = NULL;
    passwd->shell = NULL;
    passwd->comment = NULL;
    return passwd;
}

/*
 * If the buffer is a line of /etc/passwd then you can use this function to
 * parse every informations. Create the passwd_t and return it filled.
 */
static passwd_t *get_passwd(char *buff)
{
    passwd_t *passwd = init_passwd();
    char *uid = NULL;
    char *gid = NULL;

    passwd->user = my_strtok(buff, ':');
    passwd->pass = my_strtok(NULL, ':');
    uid = my_strtok(NULL, ':');
    gid = my_strtok(NULL, ':');
    passwd->comment = my_strtok(NULL, ':');
    passwd->home = my_strtok(NULL, ':');
    passwd->shell = my_strtok(NULL, ':');
    passwd->uid = my_atoi(uid);
    passwd->gid = my_atoi(gid);
    free_null_check(uid);
    free_null_check(gid);
    return passwd;
}

/*
 * Main function for getting the informations of a user with his name.
 * Open the file /etc/passwd and get every informations of it.
 * This function use a static buffer of 1024, this can be improve in the futur.
 * But generally it's impossible for a line of /etc/passwd to overflow 1024.
 */
passwd_t *get_passwd_from_user(char *username)
{
    int fd = open("/etc/passwd", O_RDONLY);
    size_t line_size = 1024;
    char line_buffer[1024] = {0};
    size_t len = read_line(fd, line_buffer, line_size);
    passwd_t *passwd = NULL;

    while (len > 0) {
        remove_newline(line_buffer);
        if (!my_strncmp(line_buffer, username, my_strlen(username))) {
            passwd = get_passwd(line_buffer);
            break;
        }
        len = read_line(fd, line_buffer, line_size);
    }
    close(fd);
    return passwd;
}

/*
 * This function will free the entire passwd structure.
 * Check before if the content is NULL or not but be careful please.
 */
void free_passwd(passwd_t *passwd)
{
    if (passwd == NULL)
        return;
    free_null_check(passwd->home);
    free_null_check(passwd->user);
    free_null_check(passwd->pass);
    free_null_check(passwd->shell);
    free_null_check(passwd->comment);
    free(passwd);
}

/*
 * DEBUG function to print the content of the structure passwd.
 * If the structure is NULL then just marked as NULL in the stdout.
 */
void print_passwd(passwd_t *passwd)
{
    if (passwd == NULL) {
        my_printf("\033[1;39mpasswd NULL\033[0m\n");
        return;
    }
    if (passwd->user != NULL)
        my_printf("%s\n", passwd->user);
    if (passwd->pass != NULL)
        my_printf("%s\n", passwd->pass);
    if (passwd->uid != -1)
        my_printf("%d\n", passwd->uid);
    if (passwd->gid != -1)
        my_printf("%d\n", passwd->gid);
    if (passwd->comment != NULL)
        my_printf("%s\n", passwd->comment);
    if (passwd->home != NULL)
        my_printf("%s\n", passwd->home);
    if (passwd->shell != NULL)
        my_printf("%s\n", passwd->shell);
}
