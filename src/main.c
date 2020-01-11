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
#include "version.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>

void display_usage(void)
{
    printf("Usage: ./my_less <file> [<file2> <file3> ...]\n\
                                [-h] [-v]\n");
}

void display_help(void)
{
    display_usage();
    printf("\nDescription:\n\
    file            file(s) to open\n\
    -h, --help      show this help\n\
    -v, --version   display the git version\n");
}

int main(int ac, char **av)
{
    unsigned int files = 0;
    lessfile_t *lf = NULL;

    for (int i = 1; i < ac; ++i) {
        if (!strcmp(av[i], "-h") || !strcmp(av[i], "--help")) {
            display_help();
            return (0);
        } else if (!strcmp(av[i], "-v") || !strcmp(av[i], "--version")) {
            display_version();
            return (0);
        } else {
            files += 1;
            lf = load_file(av[i]);
            if (lf != NULL) {
                display_file(lf);
                unload_file(lf);
            }
        }
    }
    if (files == 0) {
        display_usage();
        return (1);
    }
    return (0);
}