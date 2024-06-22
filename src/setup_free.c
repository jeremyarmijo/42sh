/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** Binary tree setup
*/

#include <stdlib.h>
#include <unistd.h>
#include "built_in.h"
#include "binary_tree.h"
#include "setup.h"

static char **create_tab(void)
{
    char **tab = malloc(sizeof(char *));

    *tab = NULL;
    return tab;
}

static void setup_r2(char **args, args_t **arg, int *values, list_crea_t *list)
{
    char **tab = create_tab();

    if (list->car == NULL && args[list->i] != NULL) {
        for (; args[list->i] != NULL && *args[list->i] != '>' &&
            *args[list->i] != '<' && *args[list->i] != '|' &&
            *args[list->i] != ';' && *args[list->i] != '&'; list->i += 1)
            change_data(&tab, "", args[list->i], "");
        *arg = malloc(sizeof(args_t));
        (*arg)->value = tab;
        (*arg)->next_l = NULL;
        (*arg)->next_r = NULL;
    } else if (list->car != NULL) {
        change_data(&tab, "", list->car, "");
        values[list->index] = list->i;
        *arg = malloc(sizeof(args_t));
        (*arg)->value = tab;
        setup_l(args, &(*arg)->next_l, values, list->i);
        setup_r(args, &(*arg)->next_r, values, list->i);
    } else
        free(tab);
}

static void setup_r(char **args, args_t **arg, int *values, size_t n)
{
    list_crea_t list = {.car = NULL, .i = n + 1, .index = 0};
    size_t tmp = 0;

    for (; args[tmp] != NULL; tmp += 1);
    for (; values[list.index] != -1; list.index += 1)
        if (values[list.index] < (int)tmp && values[list.index] > (int)n)
            tmp = values[list.index];
    for (size_t j = list.i; j < tmp; j += 1)
        if ((same_str(args[j], ">>") || same_str(args[j], ">") ||
            same_str(args[j], "<<") || same_str(args[j], "<") ||
            same_str(args[j], "|") || same_str(args[j], "||") ||
            same_str(args[j], "&") || same_str(args[j], "&&"))
            && list.car == NULL) {
            list.car = args[j];
            list.i = j;
        }
    *arg = NULL;
    setup_r2(args, arg, values, &list);
}

static void change_data_if_need(char **args, list_crea_t *list, char ***tab)
{
    for (list->i += 1; list->i < (int)list->n; list->i += 1)
        change_data(tab, "", args[list->i], "");
}

static void setup_l2(char **args, args_t **arg, int *values, list_crea_t *list)
{
    char **tab = create_tab();

    if (list->car == NULL && list->i >= 0) {
        for (; list->i >= 0 && *args[list->i] != '>' && *args[list->i] != '<'
            && *args[list->i] != '|' && *args[list->i] != ';' &&
            *args[list->i] != '&'; list->i -= 1);
        change_data_if_need(args, list, &tab);
        *arg = malloc(sizeof(args_t));
        (*arg)->value = tab;
        (*arg)->next_l = NULL;
        (*arg)->next_r = NULL;
    } else if (list->car != NULL) {
        change_data(&tab, "", list->car, "");
        values[list->index] = list->i;
        *arg = malloc(sizeof(args_t));
        (*arg)->value = tab;
        setup_l(args, &(*arg)->next_l, values, list->i);
        setup_r(args, &(*arg)->next_r, values, list->i);
    } else
        free(tab);
}

static void setup_l(char **args, args_t **arg, int *values, size_t n)
{
    list_crea_t list = {.car = NULL, .i = n - 1, .n = n, .index = 0};
    int tmp = -1;

    for (; values[list.index] != -1; list.index += 1)
        if (values[list.index] > tmp && values[list.index] < (int)n)
            tmp = values[list.index];
    for (size_t j = list.i; (int)j > tmp; j -= 1)
        if (((same_str(args[j], ">>") || same_str(args[j], ">") ||
            same_str(args[j], "<<") || same_str(args[j], "<") ||
            same_str(args[j], "|") || same_str(args[j], "||") ||
            same_str(args[j], "&") || (same_str(args[j], "&&")))
            && list.car == NULL) || (same_str(args[j], ";") &&
            (list.car == NULL || *list.car != ';'))){
            list.car = args[j];
            list.i = j;
        }
    *arg = NULL;
    setup_l2(args, arg, values, &list);
}

args_t *setup_args(char **args)
{
    size_t i = 0;
    args_t *arg = NULL;
    int *tab = NULL;

    for (; args[i] != NULL; i += 1);
    tab = malloc(sizeof(int) * (i + 1));
    for (size_t j = 0; j <= i; j += 1)
        tab[j] = -1;
    setup_l(args, &arg, tab, i);
    free(tab);
    return arg;
}

void free_tree(args_t *args)
{
    if (args != NULL) {
        for (size_t i = 0; args->value[i] != NULL; i += 1)
            free(args->value[i]);
        free(args->value);
        free_tree(args->next_l);
        free_tree(args->next_r);
        free(args);
    }
}

void free_tab(char **tab)
{
    for (size_t i = 0; tab[i] != NULL; i += 1)
        free(tab[i]);
    if (tab != NULL)
        free(tab);
}

bool error_handling(args_t *args, int *exit_status)
{
    if (args != NULL && *args->value != NULL) {
        if ((**args->value == '|' || **args->value == '&') &&
            (args->next_l == NULL || args->next_r == NULL ||
            *args->next_l->value == NULL || *args->next_r->value == NULL)) {
            write(2, "Invalid null command.\n", 22);
            *exit_status = 1;
            return true;
        }
        if ((**args->value == '>' || **args->value == '<') &&
            (args->next_l == NULL || args->next_r == NULL ||
            *args->next_l->value == NULL || *args->next_r->value == NULL)) {
            write(2, "Missing name for redirect.\n", 27);
            *exit_status = 1;
            return true;
        }
        return error_handling(args->next_l, exit_status)
            || error_handling(args->next_r, exit_status);
    } else
        return false;
}
