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

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 32
#endif

#ifndef ML_FILES_H
#define ML_FILES_H 1

struct lessfile_s {
    char **content;
    char *filepath;
    unsigned int line;
    unsigned int line_c;
};
typedef struct lessfile_s lessfile_t;

char *read_file(char *filepath);
char *get_next_line(char *str, char **endptr);
int split_lines(char *str, lessfile_t *lf);
lessfile_t *load_file(char *filepath);
void unload_file(lessfile_t *lf);

#endif // ML_FILES_H

#ifndef ML_INTERFACE_H
#define ML_INTERFACE_H 1

#define KEY_PGUP    339
#define KEY_PGDOWN  338

#define eol(c) ((c) == '\n' || (c) == '\0')
#define lpool(l, m) (l) <= (m) ? (l) : (m)

void init_screen(void);
void display_lines(lessfile_t *lf);
int go_up(lessfile_t *lf, unsigned int offset);
int go_down(lessfile_t *lf, unsigned int offset);
int handle_input(lessfile_t *lf, int *ch,
    unsigned int *prev_height, unsigned int *prev_width);
void display_file(lessfile_t *lf);

#endif // ML_INTERFACE_H

#ifndef ML_MAIN_H
#define ML_MAIN_H 1

void display_usage(void);
void display_version(void);
void display_help(void);

#endif // ML_MAIN_H