/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** history for shell
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include "history.h"
#include "linked_list.h"

list_t *free_list(list_t *head)
{
    for (list_t *temp = NULL; head != NULL; head = temp) {
        temp = head->next;
        free(head->arg);
        free(head);
    }
    return NULL;
}

void rm_list(list_t **head)
{
    list_t *temp = (*head)->next;

    free((*head)->arg);
    free(*head);
    *head = temp;
    if (*head != NULL)
        (*head)->prev = NULL;
}

static void add_list(list_t **head, char *str)
{
    list_t *new_node = malloc(sizeof(list_t));

    new_node->arg = malloc(sizeof(char) * (my_strlen(str) + 1));
    *new_node->arg = '\0';
    my_strcat(1, new_node->arg, str);
    new_node->prev = NULL;
    new_node->next = *head;
    if (*head != NULL)
        (*head)->prev = new_node;
    *head = new_node;
}

static void get_letters(list_t **head, list_t **node, size_t index, char c)
{
    char *nw_word = NULL;
    bool len = (c != BACKSLASH);

    if (c == '\n' || (my_strlen((*node)->arg) == 0 && !len))
        return;
    if (c == '\t' && isatty(STDIN_FILENO))
        c = '\0';
    if (*head != *node) {
        rm_list(head);
        add_list(head, (*node)->arg);
        *node = *head;
    }
    nw_word = malloc(sizeof(char) * (my_strlen((*head)->arg) + (2 * len) + 1));
    *nw_word = '\0';
    my_strncat(nw_word, (*head)->arg, my_strlen((*head)->arg) - index - !len);
    my_strncat(nw_word, &c, 1 - !len);
    my_strncat(nw_word, (*head)->arg + my_strlen((*head)->arg) - index, index);
    free((*head)->arg);
    (*head)->arg = nw_word;
}

static void get_arrow_keys(list_t **head, list_t **node, size_t *index, char c)
{
    list_t *temp = *node;

    if (c == 'A') {
        while ((*node)->next && !same_begin((*node)->next->arg, (*head)->arg))
            *node = (*node)->next;
        if ((*node)->next != NULL)
            *node = (*node)->next;
        else
            *node = temp;
    }
    if (c == 'B') {
        while ((*node)->prev && !same_begin((*node)->prev->arg, (*head)->arg))
            *node = (*node)->prev;
        if ((*node)->prev != NULL)
            *node = (*node)->prev;
        else
            *node = temp;
    }
    *index -= (c == 'C') ? (*index > 0) : (0);
    *index += (c == 'D') ? (*index < my_strlen((*node)->arg)) : (0);
}

static void print_string(list_t *node, list_t *node_comp, size_t index)
{
    char *path = "";
    size_t i = 0;

    if (node_comp == NULL || *node_comp->arg == '\0')
        printf("%s", node->arg);
    else {
        for (i = my_strlen(node->arg); i > 0 &&
            !is_sep(node->arg[i - 1]) && node->arg[i - 1] != '/'; i -= 1);
        for (size_t j = 0; j < i; j += 1) {
            printf("%c", node->arg[j]);
        }
    }
    if (node_comp != NULL)
        path = (*node_comp->arg == '\e') ?
            (supr_color(node_comp->arg)) : (node_comp->arg);
    printf("%s", path);
    if (index > 0)
        printf("\033[%ldD", index);
    if (node_comp != NULL && *node_comp->arg == '\e')
        free(path);
}

static void loop_body(list_t ***tab, size_t *index, char c, bool tty)
{
    if (c == '\e') {
        c = getchar();
        c = getchar();
        if (tty)
            get_arrow_keys(tab[HEAD], tab[NODE], index, c);
    } else
        get_letters(tab[HEAD], tab[NODE], *index, c);
    if (tty) {
        printf("\0338\033[0J");
        if (*tab[HEAD_COMP] != *tab[NODE_COMP]) {
            printf("\n");
            print_comp((*tab[HEAD_COMP])->next, *tab[NODE_COMP]);
        }
        printf("\0338");
        fflush(stdin);
        print_string(*tab[NODE], *tab[NODE_COMP], *index);
    }
}

static void loop_it(list_t ***tab, char **old_path, bool *end, bool tty)
{
    size_t index = 0;

    for (char c = getchar(); c != '\0' && (c != '\n' || tty); c = getchar()) {
        if (c == ERR || c == CTRL_D) {
            *end = true;
            break;
        }
        if (tty && update_list(tab, *tab[HEAD_COMP], old_path, c))
            break;
        loop_body(tab, &index, c, tty);
    }
}

static list_t *make_tab(list_t **head, list_t *commands, bool *end)
{
    list_t *head_comp = NULL;
    list_t **tab[] = {head, &(list_t *){*head}, &head_comp,
        &(list_t *){head_comp}, &commands, NULL};
    char *old_path = malloc(sizeof(char));

    *old_path = '\0';
    loop_it(tab, &old_path, end, isatty(STDIN_FILENO));
    cut_list(tab[HEAD_COMP], old_path);
    free_list(head_comp);
    free_list(commands);
    free(old_path);
    return *tab[NODE];
}

void get_command(list_t **head, char *path, bool *end)
{
    list_t *node = NULL;
    struct termios term[2] = {{0}, {0}};
    list_t *commands = NULL;

    if (isatty(STDIN_FILENO))
        find_commands(&commands, path);
    free(path);
    add_list(head, "");
    tcgetattr(STDIN_FILENO, &term[0]);
    term[1] = term[0];
    term[1].c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term[1]);
    node = make_tab(head, commands, end);
    if (isatty(STDIN_FILENO) && (*end == false || my_strlen(node->arg) > 0))
        printf("\0338\033[0J%s\n", node->arg);
    tcsetattr(STDIN_FILENO, TCSANOW, &term[0]);
    if (*head != node) {
        rm_list(head);
        add_list(head, node->arg);
    }
}
