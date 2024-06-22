/*
** EPITECH PROJECT, 2024
** built_in
** File description:
** .h for 42sh
*/

#ifndef BUILT_IN
    #define BUILT_IN

    #include <stddef.h>
    #include <stdbool.h>

    #define ENV setup->env

typedef struct alias_s {
    char *key;
    char *value;
    struct alias_s *next;
} alias_t;

typedef struct shell_setup_s {
    char ***env;
    alias_t *alias_database;
} shell_setup_t;

void sort_alias(alias_t *);
char *add_str_to_str(char *, char *);
void verif_alias(shell_setup_t *, char ***);
bool alias(char **, shell_setup_t *);
shell_setup_t *init_setup_shell(char ***);

#endif
