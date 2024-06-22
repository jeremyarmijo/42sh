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
#include "completion.h"

static char *supr_bg(char *buff)
{
    char *temp = NULL;
    size_t i = 0;

    if (same_begin(buff, "\e[") && find_str(buff, ';')) {
        for (i = 2; buff[i] != ';' && buff[i] != 'm'; i += 1);
        if (buff[i] == 'm')
            i = 0;
        else
            i += 1;
        temp = malloc(sizeof(char) * (my_strlen(buff) - i + 3));
        *temp = '\0';
        my_strcat(2, temp, "\e[", buff + i);
        free(buff);
        return temp;
    }
    return buff;
}

void is_select(list_t *head, char *str, char c)
{
    char *path = NULL;
    size_t i = 0;

    if (is_sep(c))
        str = "";
    if (find_str(str, '/')) {
        for (i = my_strlen(str) - 1; str[i] != '/'; i -= 1);
        i += 1;
    }
    for (head = head->next; head != NULL; head = head->next) {
        path = (*head->arg == '\e') ? (supr_color(head->arg)) : (head->arg);
        head->is_ok = same_begin(path, str + i);
        if (*head->arg == '\e')
            free(path);
    }
}

static char *comp_size(list_t *head, size_t *tab, size_t lines)
{
    struct winsize window = {0};
    char *path = NULL;
    size_t nb = 0;
    size_t len = 0;

    ioctl(STDIN_FILENO, TIOCGWINSZ, &window);
    for (head = head->next; head != NULL; head = head->next) {
        if (!head->is_ok || *head->arg == '\0')
            continue;
        path = supr_color(head->arg);
        len = (len < my_strlen(path)) ? (my_strlen(path)) : (len);
        free(path);
        nb += 1;
    }
    lines = ceilf((float)nb / (window.ws_col / (len + 2)));
    tab[0] = (lines + 1 > window.ws_row) ? (0) : (ceilf((float)nb / lines));
    tab[1] = len;
    if (tab[0] == 0)
        printf("\033[1;2;3;31mToo many possibilties to show!\033[0m");
    return NULL;
}

void print_comp(list_t *head, list_t *node)
{
    size_t tab[2] = {0};
    char *path = comp_size(head, tab, 0);
    size_t len = 0;
    size_t nb = 0;

    for (head = head->next; tab[0] > 0 && head != NULL; head = head->next) {
        if (!head->is_ok || *head->arg == '\0')
            continue;
        path = supr_color(head->arg);
        len = tab[1] - my_strlen(path);
        free(path);
        path = malloc(sizeof(char) * (len + 1));
        for (size_t i = 0; i < len; i += 1)
            path[i] = ' ';
        path[len] = '\0';
        printf("%2$s%1$s%2$s%3$s\033[0m%4$s", head->arg, (node == head) ?
            ("\033[30;47m") : (""), path, (nb < tab[0] - 1) ? ("  ") : ("\n"));
        nb = (nb < tab[0] - 1) ? (nb + 1) : (0);
        free(path);
    }
}

static void add_node(char *buff, list_t **node, size_t len)
{
    char *temp = NULL;

    (*node)->next = malloc(sizeof(list_t));
    (*node)->next->prev = *node;
    *node = (*node)->next;
    (*node)->next = NULL;
    (*node)->is_ok = false;
    buff[len - 1] = '\0';
    if (same_begin(buff, "\e[0m")) {
        temp = malloc(sizeof(char) * (my_strlen(buff) - 4 + 1));
        *temp = '\0';
        my_strcat(1, temp, buff + 4);
        free(buff);
        buff = temp;
    }
    (*node)->arg = supr_bg(buff);
}

static list_t *get_ls(list_t *head, FILE *fd)
{
    list_t *node = head;
    char *buff = NULL;

    *head->arg = '\0';
    node->next = malloc(sizeof(list_t));
    node->next->prev = node;
    node = node->next;
    node->next = NULL;
    node->is_ok = false;
    node->arg = malloc(sizeof(char));
    *node->arg = '\0';
    for (ssize_t len = 0; true; buff = NULL) {
        len = getline(&buff, &(size_t){0}, fd);
        if (len == -1)
            break;
        add_node(buff, &node, len);
    }
    free(buff);
    pclose(fd);
    return head;
}

list_t *ls_list(list_t *head, char *src, char *command, size_t len)
{
    FILE *fd = NULL;
    char path[4096] = {0};
    char temp[4096] = {0};

    if (find_str(src, '/') && my_strlen(src) > 1) {
        for (; len > 0 && src[len] != '/'; len -= 1);
        command = (src[len + 1] == '.') ? ("-a ") : ("");
    }
    my_strcat(2, path, "ls --color ", command);
    fd = (*src == '\0' || is_dir(my_strncat(temp, src, len + 1))) ?
        (popen(my_strncat(path, src, len + 1), "r")) : (NULL);
    if (fd != NULL) {
        free_list(head);
        head = malloc(sizeof(list_t));
        head->prev = NULL;
        head->next = NULL;
        head->is_ok = false;
        head->arg = malloc(sizeof(char));
    }
    return (fd == NULL) ? (head) : (get_ls(head, fd));
}

void path_tab(list_t **head, char **tab, char **old_path, char *path)
{
    list_t *temp = *head;
    char c = path[my_strlen(path) - 1];
    size_t len = 0;

    for (; tab[len] != NULL; len += 1);
    *head = ls_list(*head, tab[len - 1],
        (*tab[len - 1] == '.') ? ("-a ") : (""), my_strlen(tab[len - 1]) - 1);
    if (is_sep(c) || *head == temp)
        *head = ls_list(*head, "", "", -1);
    if (*head != temp) {
        free(*old_path);
        *old_path = supr_color(path);
    }
    if (is_dir(tab[len - 1]) && !is_sep(c) && c != '/' && c != '.')
        my_strcat(1, path, "/");
    else
        is_select(*head, tab[len - 1], c);
}

static bool change_node(list_t *head, list_t **node, list_t *head_comp,
    size_t len)
{
    char *path = supr_color((*node)->arg);
    char *nw_word = malloc(sizeof(char) *
        (my_strlen(head->arg) + my_strlen(path) + 2));
    char **tab = NULL;

    *nw_word = '\0';
    for (len = my_strlen(head->arg); len > 0 &&
        !is_sep(head->arg[len - 1]) && head->arg[len - 1] != '/'; len -= 1);
    my_strncat(nw_word, head->arg, len);
    my_strcat(1, nw_word, path);
    free(path);
    tab = my_str_to_word_array(nw_word);
    for (len = 0; tab[len] != NULL; len += 1);
    my_strcat(1, nw_word, (is_dir(tab[len - 1]) &&
        tab[len - 1][my_strlen(tab[len - 1]) - 1] != '/') ? ("/") : (""));
    free(head->arg);
    head->arg = nw_word;
    free_tab(tab);
    *node = head_comp;
    return false;
}

bool update_list(list_t ***tab, list_t *head, char **old_path, char c)
{
    bool is_all_ok = false;

    if (c == '\t' && *(*tab[NODE])->arg != '\0') {
        new_tab(tab[HEAD_COMP], *tab[COMANDS], old_path, (*tab[NODE])->arg);
        *tab[NODE_COMP] = (head != *tab[HEAD_COMP]) ?
            (*tab[HEAD_COMP]) : (*tab[NODE_COMP]);
        if ((*tab[HEAD_COMP]) == NULL || (*tab[HEAD_COMP])->next->next == NULL)
            return false;
        for (list_t *tmp = (*tab[HEAD_COMP])->next->next; tmp; tmp = tmp->next)
            is_all_ok = (tmp->is_ok) ? (true) : (is_all_ok);
        if (is_all_ok)
            *tab[NODE_COMP] = ((*tab[NODE_COMP])->next) ?
                ((*tab[NODE_COMP])->next) : ((*tab[HEAD_COMP])->next->next);
        while (is_all_ok && !(*tab[NODE_COMP])->is_ok)
            *tab[NODE_COMP] = ((*tab[NODE_COMP])->next) ?
                ((*tab[NODE_COMP])->next) : ((*tab[HEAD_COMP])->next->next);
    } else
        if (*tab[NODE_COMP] != *tab[HEAD_COMP] && c != '\t')
            return change_node(*tab[HEAD], tab[NODE_COMP], *tab[HEAD_COMP], 0);
    return (*tab[NODE_COMP] == *tab[HEAD_COMP] && c == '\n');
}
