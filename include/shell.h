/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** .h for shell
*/

#ifndef MINISHELL_H
    #define MINISHELL_H

    #include <stdbool.h>
    #include "built_in.h"
    #include "binary_tree.h"

typedef struct list_s {
    struct list_s *next;
    struct list_s *prev;
    char *arg;
} list_t;

bool my_env(shell_setup_t *, char **, size_t _);
bool built_in(int *, char *, shell_setup_t *, char **);;
void change_output(char *);
size_t error_value(size_t _);
bool is_alpha(char *);
bool my_str(char *, char *);
bool find_str(char *, char);
char **my_str_to_word_array(char *);
char *my_strcat(size_t _, char *, ...);
char *my_strncat(char *, char *, size_t _);
bool same_str(char *, char *);
size_t my_strlen(char *);
args_t *setup_args(char **);
void search_data(shell_setup_t *, args_t *, int *);
bool error_handling(args_t *, int *);
void free_tree(args_t *);
void free_tab(char **);
void free_alias_list(shell_setup_t *);
list_t *free_list(list_t *);
void rm_list(list_t **);
void get_command(list_t **, char *, bool *);
char *get_pwd(char **, char *);
char *get_branch(char *);
char *search_path(char **, char *);

#endif
