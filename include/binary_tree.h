/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** .h for shell
*/

#ifndef BINARY_TREE_H
    #define BINARY_TREE_H

typedef struct args_s {
    struct args_s *prev;
    struct args_s *next_l;
    struct args_s *next_r;
    char **value;
} args_t;

#endif
