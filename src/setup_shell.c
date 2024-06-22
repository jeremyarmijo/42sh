/*
** EPITECH PROJECT, 2024
** setup
** File description:
** shell
*/
#include <stdlib.h>
#include "built_in.h"

shell_setup_t *init_setup_shell(char ***env)
{
    shell_setup_t *setup = malloc(sizeof(shell_setup_t));

    setup->env = env;
    setup->alias_database = NULL;
    return setup;
}
