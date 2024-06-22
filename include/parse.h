/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** .h for parsing
*/

#ifndef PARSE_H
    #define PARSE_H

    #include <stdbool.h>
    #include "binary_tree.h"
    #include "built_in.h"

void shell(shell_setup_t *, char **, char *, int *);
bool options(shell_setup_t *, char **, char *, int *);
bool same_str(char *, char *);
bool my_str(char *, char *);
char *my_strcat(size_t _, char *, ...);
size_t my_strlen(char *);
char *search_path(char **, char *);
void background(shell_setup_t *, args_t *, int *);
void operator_or(shell_setup_t *, args_t *, int *);
void operator_and(shell_setup_t *, args_t *, int *);
void search_data(shell_setup_t *, args_t *, int *);
void semi_column(shell_setup_t *, args_t *, int *);
void pipe_s(shell_setup_t *, args_t *, int *);
void arrow_r(shell_setup_t *, args_t *, int *);
void arrow_2r(shell_setup_t *, args_t *, int *);
void arrow_l(shell_setup_t *, args_t *, int *);
void arrow_2l(shell_setup_t *, args_t *, int *);

static void (*RDRCT[])(shell_setup_t *, args_t *, int *) = {
    &semi_column, &pipe_s, &arrow_r, &arrow_2r, &arrow_l, &arrow_2l,
    &operator_and, &operator_or, &background};
static char *REDIR[] = {";", "|", ">", ">>", "<", "<<", "&&", "||", "&", NULL};

#endif
