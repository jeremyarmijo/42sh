/*
** EPITECH PROJECT, 2023
** sokoban
** File description:
** game in terminal
*/
#include <stdlib.h>
#include "shell.h"
#include "built_in.h"

char *my_strcpy_j(char *dest, const char *src, int i)
{
    int bis = 0;

    while (src[bis] != '\0') {
        dest[bis + i] = src[bis];
        bis++;
    }
    dest[i + bis] = '\0';
    return dest;
}

char *add_str_to_str(char *add, char *str)
{
    int len = my_strlen(str);
    int len2 = my_strlen(add);
    char *new = malloc(sizeof(char) * (len + len2 + 1));
    int i = 0;
    int tmp = 0;

    new = my_strcpy_j(new, add, 0);
    for (; new[i] != '\0'; i++);
    for (; ((tmp + i) < (len + len2))
        && str[tmp] != '\0'; tmp++) {
        new[tmp + i] = str[tmp];
    }
    new[tmp + i] = '\0';
    free(add);
    return new;
}
