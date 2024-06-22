/*
** EPITECH PROJECT, 2024
** alias
** File description:
** fonction
*/
#include "built_in.h"
#include "shell.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

static void alias_swap(alias_t *alias, alias_t *ref_value)
{
    char *key = NULL;
    char *value = NULL;

    if ((alias)->key[0] > (ref_value)->key[0]) {
        key = strdup(alias->key);
        value = strdup(alias->value);
        free(alias->key);
        free(alias->value);
        alias->key = strdup(ref_value->key);
        alias->value = strdup(ref_value->value);
        free(ref_value->key);
        free(ref_value->value);
        ref_value->key = strdup(key);
        ref_value->value = strdup(value);
    }
}

void sort_alias(alias_t *alias)
{
    alias_t *ref_value = alias;

    for (; alias; alias = (alias)->next) {
        for (; ref_value; ref_value = ref_value->next)
            alias_swap(alias, ref_value);
    }
}

static void print_alias_list(shell_setup_t *setup)
{
    alias_t *tmp = (alias_t *)setup->alias_database;

    for (; tmp; tmp = tmp->next) {
        printf("%s='%s'\n",
            tmp->key, tmp->value);
    }
}

void print_value(char **args, shell_setup_t *setup)
{
    alias_t *tmp = (alias_t *)setup->alias_database;

    for (; tmp; tmp = tmp->next)
        if (same_str(tmp->key, args[1])) {
            printf("%s\n", tmp->value);
            break;
        }
}

int add_alias(char **args, shell_setup_t **setup)
{
    alias_t *head = (*setup)->alias_database;
    alias_t *final = malloc(sizeof(alias_t));

    final->key = strdup(args[1]);
    final->value = strdup(args[2]);
    final->next = NULL;
    for (size_t i = 3; args[i]; i++) {
        final->value = add_str_to_str(
            final->value, " ");
        final->value = add_str_to_str(
            final->value, args[i]);
    }
    if (!(*setup)->alias_database)
        (*setup)->alias_database = final;
        else {
            for (; head->next; head = head->next);
            head->next = final;
        }
    return 0;
}

void add_value(alias_t *alias, char **args)
{
    for (size_t i = 3; args[i]; i++) {
        alias->value = add_str_to_str(
            alias->value, " ");
        alias->value = add_str_to_str(
            alias->value, args[i]);
    }
}

static bool search_alias(char **args, shell_setup_t **setup)
{
    alias_t *tmp = (alias_t *)(*setup)->alias_database;

    for (; tmp; tmp = tmp->next)
        if (same_str(tmp->key, args[1])) {
            free(tmp->value);
            tmp->value = strdup(args[2]);
            add_value(tmp, args);
            return true;
        }
    return false;
}

static bool error_alias(char **args)
{
    for (size_t i = 1; args[i]; i++)
        if (same_str(args[i], "alias")) {
            write(2, "alias: Too dangerous to alias that.\n", 36);
            return true;
        }
    return false;
}

bool alias(char **args, shell_setup_t *setup)
{
    size_t nb_args = -1;

    for (; args[nb_args + 1]; nb_args++);
    if (nb_args == 0)
        print_alias_list(setup);
    if (nb_args == 1)
        print_value(args, setup);
    if (nb_args > 1) {
        if (search_alias(args, &setup))
            return false;
        if (error_alias(args))
            return true;
        add_alias(args, &setup);
    }
    return false;
}
