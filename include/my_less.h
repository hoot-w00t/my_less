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

#ifndef MY_LESS_H
#define MY_LESS_H

#define BUFFER_SIZE 32
#define KEY_PGUP    339
#define KEY_PGDOWN  338
#define eol(c) ((c) == '\n' || (c) == '\0')

// files.c

struct lessfile_s {
    char *content;
    unsigned int line;
    unsigned int total_lines;
};
typedef struct lessfile_s lessfile_t;
char *read_file(char *filepath);
lessfile_t *load_file(char *filepath);
void unload_file(lessfile_t *lf);

// end of files.c

// interface.c

void init_screen(void);
void display_lines(lessfile_t *lf);
int handle_input(lessfile_t *lf, int *ch,
    unsigned int *height, unsigned int *width);
void display_file(lessfile_t *lf);

// end of interface.c

// main.c

void display_usage(void);
void display_version(void);
void display_help(void);

// end of main.c

#endif