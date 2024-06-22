/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** .h for prompt.c
*/

#ifndef PROMPT_H
    #define PROMPT_H
    #include <stdbool.h>
    #include <stdio.h>

bool my_str(char *, char *);
bool same_str(char *, char *);
bool find_str(char *, char);
size_t my_strlen(char *);
char *my_strcat(size_t _, char *, ...);
char *my_strncat(char *, char *, size_t _);

#endif
