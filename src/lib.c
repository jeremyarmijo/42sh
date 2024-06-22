/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** lib functions
*/

#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <unistd.h>

size_t my_strlen(char *str)
{
    size_t i = 0;

    if (str == NULL)
        return 0;
    for (i = 0; str[i] != '\0'; i += 1);
    return i;
}

bool is_alpha(char *str)
{
    if ((*str < 'A' || (*str > 'Z' && *str < 'a') || *str > 'z') &&
        *str != '_') {
        write(2, "setenv: Variable name must begin with a letter.\n", 48);
        return false;
    }
    for (size_t i = 0; str[i] != '\0'; i += 1) {
        if ((str[i] < '0' || (str[i] > '9' && str[i] < 'A') ||
            (str[i] > 'Z' && str[i] < 'a') || str[i] > 'z') && str[i] != '_') {
            write(2, "setenv: Variable name must "
                "contain alphanumeric characters.\n", 60);
            return false;
        }
    }
    return true;
}

bool same_str(char *str1, char *str2)
{
    size_t i = 0;

    for (; str1[i] != '\0'; i += 1)
        if (str1[i] != str2[i])
            return false;
    return str2[i] == '\0';
}

char *my_strncat(char *dest, char *src, size_t n)
{
    size_t i = 0;
    size_t y = 0;

    for (i = 0; dest[i] != '\0'; i += 1);
    for (y = 0; y < n && src[y] != '\0'; y += 1)
        dest[i + y] = src[y];
    dest[i + y] = '\0';
    return dest;
}

char *my_strcat(size_t nb_args, char *dest, ...)
{
    size_t i = 0;
    char *src = NULL;
    va_list ap = {0};

    va_start(ap, dest);
    for (i = 0; dest[i] != '\0'; i += 1);
    for (; nb_args > 0; nb_args -= 1) {
        src = va_arg(ap, char *);
        for (size_t y = 0; src[y] != '\0'; y += 1) {
            dest[i] = src[y];
            i += 1;
        }
    }
    dest[i] = '\0';
    va_end(ap);
    return dest;
}
