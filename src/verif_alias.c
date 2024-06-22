/*
** EPITECH PROJECT, 2024
** replace the cmd
** File description:
** if it existe as alias in the database
*/
#include <string.h>
#include <stdlib.h>
#include "built_in.h"
#include "shell.h"

int nb_word(char *str, char ref)
{
    int nb = 0;
    int i = 0;

    if (str == NULL)
        return 0;
    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] != ref && str[i + 1] == ref)
            nb++;
        if (str[i] != ref && str[i + 1] == '\0')
            nb++;
    }
    return nb;
}

int nb_strings(char **tab)
{
    size_t i = 0;

    for (; tab[i]; i++);
    return i;
}

void add_strings(alias_t *alias, char ***args)
{
    char *new_string = NULL;
    char **new_arg = NULL;

    if (same_str(alias->key, (*args)[0]))
        new_string = strdup(alias->value);
        else
            new_string = strdup((*args)[0]);
    for (size_t i = 1; (*args)[i]; i++) {
        if (same_str(alias->key, (*args)[i])) {
            new_string = add_str_to_str(new_string, " ");
            new_string = add_str_to_str(new_string, alias->value);
        } else {
            new_string = add_str_to_str(new_string, " ");
            new_string = add_str_to_str(new_string, (*args)[i]);
        }
    }
    free_tab(*args);
    new_arg = my_str_to_word_array(new_string);
    *args = new_arg;
    free(new_string);
}

bool replace_correct_value(alias_t *alias, char ***args, size_t i)
{
    if (same_str(alias->key, (*args)[i])) {
        add_strings(alias, args);
        return true;
    }
    return false;
}

void verif_alias(shell_setup_t *setup, char ***args)
{
    alias_t *alias = setup->alias_database;

    if (!(*args))
        return;
    for (size_t i = 0; (*args)[i]; i++)
        if (same_str((*args)[i], "alias"))
            return;
    for (size_t i = 0; (*args)[i]; i++)
        for (alias_t *current = alias; current; current = current->next)
            replace_correct_value(current, args, i);
}
