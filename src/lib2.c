/*
** EPITECH PROJECT, 2024
** B-PSU-200-PAR-2-1-42sh-pierre-yves.guiraudet
** File description:
** lib2
*/

#include <stdbool.h>
#include <stdio.h>
#include <dirent.h>

bool is_sep(char c)
{
    return (c == ' ' || c == ';' || c == '>' ||
        c == '<' || c == '|' || c == '&' || c == '\t');
}

bool is_dir(char *str)
{
    DIR *dir = opendir(str);

    if (dir == NULL)
        return false;
    closedir(dir);
    return true;
}

bool find_str(char *str, char to_find)
{
    for (size_t i = 0; str[i] != '\0'; i += 1)
        if (str[i] == to_find)
            return true;
    return false;
}

bool my_str(char *str, char *to_find)
{
    size_t i = 0;

    for (; to_find[i] != '\0'; i += 1)
        if (to_find[i] != str[i])
            return false;
    if (str[i] != '=')
        return false;
    return true;
}

bool same_begin(char *str, char *begin)
{
    for (size_t i = 0; begin[i] != '\0'; i += 1)
        if (begin[i] != str[i])
            return false;
    return true;
}
