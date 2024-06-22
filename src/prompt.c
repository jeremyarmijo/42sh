/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** prompt functions
*/

#include "prompt.h"
#include <stdlib.h>
#include <unistd.h>

static char *get_folder_branch(char *branch, FILE *fd)
{
    char *buff = NULL;
    size_t len_get = 0;
    ssize_t i = 0;

    i = getline(&buff, &len_get, fd);
    fclose(fd);
    if (i == -1)
        return branch;
    buff[i - 1] = '\0';
    for (i = my_strlen(buff); i > 0 && buff[i - 1] != '/'; i -= 1);
    my_strcat(3, branch, "\033[34mgit:(\033[31m", buff + i, "\033[34m) ");
    free(buff);
    return branch;
}

char *get_branch(char *branch)
{
    char buff[4096] = {0};
    char path[4096] = {0};
    FILE *fd = NULL;
    ssize_t i = 0;

    *branch = '\0';
    fd = fopen(".git/HEAD", "r");
    if (fd != NULL)
        return get_folder_branch(branch, fd);
    getcwd(buff, 4096);
    for (i = my_strlen(buff); i >= 0; i -= 1) {
        for (; i > 0 && buff[i - 1] != '/'; i -= 1);
        *path = '\0';
        my_strncat(path, buff, i);
        my_strcat(1, path, ".git/HEAD");
        fd = fopen(path, "r");
        if (fd != NULL)
            return get_folder_branch(branch, fd);
    }
    return branch;
}

char *get_pwd(char **env, char *pwd)
{
    char buff[4096] = {0};
    size_t home = 0;
    ssize_t i = 0;

    *pwd = '\0';
    if (getcwd(buff, 4096) == NULL)
        return pwd;
    for (i = my_strlen(buff); i >= 0 && buff[i - 1] != '/'; i -= 1);
    for (home = 0; env[home] != NULL && !my_str(env[home], "HOME"); home += 1);
    if (env[home] != NULL && same_str(env[home] + 5, buff))
        my_strcat(1, pwd, "~");
    else if (!find_str(buff + 1, '/'))
        my_strcat(1, pwd, buff);
    else
        my_strcat(1, pwd, buff + i);
    return pwd;
}
