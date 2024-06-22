/*
** EPITECH PROJECT, 2024
** B-PSU-200-PAR-2-1-42sh-pierre-yves.guiraudet
** File description:
** completion
*/

#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "linked_list.h"
#include "func_comp.h"

char *supr_color(char *buff)
{
    char *str = NULL;
    size_t len = 0;

    while (*buff == '\e') {
        for (; *buff != 'm' && *buff != '\0'; buff += 1);
        buff += (*buff != '\0');
    }
    for (; buff[len] != '\e' && buff[len] != '\0'; len += 1);
    str = malloc(sizeof(char) * (len + 1));
    *str = '\0';
    my_strncat(str, buff, len);
    return str;
}

char *search_path(char **env, char *str)
{
    char *paths = NULL;
    size_t i = 0;

    for (i = 0; env[i] != NULL && !my_str(env[i], str); i += 1);
    if (env[i] != NULL) {
        paths = malloc(sizeof(char) * (my_strlen(env[i]) - 4));
        *paths = '\0';
        my_strcat(1, paths, env[i] + 5);
    } else if (same_str("PATH", str)){
        paths = malloc(sizeof(char) * (my_strlen("/usr/bin:/bin") + 1));
        *paths = '\0';
        my_strcat(1, paths, "/usr/bin:/bin");
    }
    return paths;
}

static list_t *get_ls(list_t *node, FILE *fd)
{
    char *buff = NULL;

    for (ssize_t len = 0; true; buff = NULL) {
        len = getline(&buff, &(size_t){0}, fd);
        if (len == -1)
            break;
        buff[len - 1] = '\0';
        node->next = malloc(sizeof(list_t));
        node->next->prev = node;
        node = node->next;
        node->next = NULL;
        node->is_ok = false;
        node->arg = buff;
    }
    free(buff);
    pclose(fd);
    return node;
}

static list_t *ls_list(list_t **head, list_t *node, char *path)
{
    char temp[4096] = {0};
    FILE *fd = (is_dir(path)) ?
        (popen(my_strcat(2, temp, "ls ", path), "r")) : (NULL);

    if (fd == NULL)
        return *head;
    if (*head != NULL)
        return get_ls(node, fd);
    *head = malloc(sizeof(list_t));
    (*head)->prev = NULL;
    (*head)->is_ok = false;
    (*head)->arg = malloc(sizeof(char));
    *(*head)->arg = '\0';
    (*head)->next = malloc(sizeof(list_t));
    (*head)->next->prev = *head;
    (*head)->next->is_ok = false;
    (*head)->next->arg = malloc(sizeof(char));
    *(*head)->next->arg = '\0';
    (*head)->next->next = NULL;
    return get_ls((*head)->next, fd);
}

void find_commands(list_t **head, char *path)
{
    list_t *node = NULL;
    char temp[4096] = {0};

    for (ssize_t i = 1; *path != '\0'; i += 1) {
        if (path[i] == ':' || path[i] == '\0') {
            *temp = '\0';
            my_strncat(temp, path, i);
            node = ls_list(head, node, temp);
            path += i + (*(path + i) != '\0');
            i = 0;
        }
    }
}

static void add_commands(list_t **head, list_t *command, char **old_path,
    char *path)
{
    char **tab = my_str_to_word_array(path);
    list_t *node = NULL;
    char c = path[my_strlen(path) - 1];
    ssize_t len = 0;
    ssize_t pos = 0;

    for (; tab[len] != NULL; len += 1);
    for (pos = len - 1; pos >= 0 && !is_sep(*tab[pos]); pos -= 1);
    pos -= (c == ' ' || c == '\t') ? (1) : (0);
    if (pos >= len - 2) {
        for (node = *head; node && node->next != NULL; node = node->next);
        is_select(command, (is_sep(*tab[len - 1])) ? ("") : (tab[len - 1]), 0);
        if (node == NULL)
            *head = command;
        else
            node->next = command->next;
        free(*old_path);
        *old_path = supr_color(path);
    }
    free_tab(tab);
}

void cut_list(list_t **head, char *old_path)
{
    char **tab = my_str_to_word_array(old_path);
    list_t *node = NULL;
    size_t len = 0;

    for (; tab != NULL && tab[len] != NULL; len += 1);
    if (*head != NULL)
        node = (*head)->next;
    for (; node && node->next && *node->next->arg != '\0'; node = node->next);
    if (!node || (!node->next && tab != NULL && is_sep(*tab[len - 1])))
        *head = NULL;
    else
        node->next = NULL;
    if (tab != NULL)
        free_tab(tab);
}

void new_tab(list_t **head, list_t *commands, char **old_path, char *path)
{
    char **tab = NULL;
    size_t len = 0;

    if (same_str(*old_path, path))
        return;
    tab = my_str_to_word_array(path);
    for (; tab != NULL && tab[len] != NULL; len += 1);
    cut_list(head, *old_path);
    if (tab != NULL && !is_sep(*tab[len - 1]))
        path_tab(head, tab, old_path, path);
    else
        *head = free_list(*head);
    if (tab != NULL) {
        if (!find_str(tab[len - 1], '/'))
            add_commands(head, commands, old_path, path);
        free_tab(tab);
    }
}
