/*
    my_less
    Copyright (C) 2020  akrocynova

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#include "my_less.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

void display_usage(void)
{
    printf("Usage: ./my_less <file> [--help]\n");
}

void display_help(void)
{
    display_usage();
    printf("\nDescription:\n\
    file        file to open\n\
    --help      show this help\n");
    exit(0);
}

int main(int ac, char **av)
{
    lessfile_t *lf = NULL;

    for (int i = 1; i < ac; ++i) {
        if (!strcmp(av[i], "--help")) {
            display_help();
        } else if (lf == NULL) {
            lf = load_file(av[i]);
        }
    }
    if (lf == NULL) {
        display_usage();
    } else {
        display_file(lf);
        unload_file(lf);
    }
    return (0);
}