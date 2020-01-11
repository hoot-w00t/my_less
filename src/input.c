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
#include <ncurses.h>

int go_up(lessfile_t *lf, unsigned int offset)
{
    if (lf->line == 0) {
        beep();
        return (0);
    }
    if (offset > lf->line) {
        lf->line = 0;
    } else {
        lf->line -= offset;
    }
    return (1);
}

int go_down(lessfile_t *lf, unsigned int offset)
{
    unsigned int scr_lines = getmaxy(stdscr) - 1;

    if (lf->line == lf->line_c - 1 || lf->line + scr_lines >= lf->line_c) {
        beep();
        return (0);
    }
    if (lf->line + offset + scr_lines >= lf->line_c) {
        lf->line = lf->line_c - scr_lines;
    } else {
        lf->line += offset;
    }
    return (1);
}

int go_right(lessfile_t *lf, unsigned int offset)
{
    if (lf->column + offset >= lf->column_max) {
        beep();
        return (0);
    }
    lf->column += offset;
    return (1);
}

int go_left(lessfile_t *lf, unsigned int offset)
{
    if (lf->column == 0) {
        beep();
        return (0);
    }
    if (offset > lf->column) {
        lf->column = 0;
    } else {
        lf->column -= offset;
    }
    return (1);
}

int handle_input(lessfile_t *lf, int *ch,
    unsigned int *prev_height, unsigned int *prev_width)
{
    unsigned int height = 0;
    unsigned int width = 0;

    getmaxyx(stdscr, height, width);
    switch (*ch) {
        case KEY_DOWN: return go_down(lf, 1);
        case KEY_PGDOWN: return go_down(lf, height - 1);
        case KEY_UP: return go_up(lf, 1);
        case KEY_PGUP: return go_up(lf, height - 1);
        case KEY_RIGHT: return go_right(lf, width / 2);
        case KEY_LEFT: return go_left(lf, width / 2);
        case 'q': return (-1);
    }

    if (height != *prev_height || width != *prev_width) {
        *prev_height = height;
        *prev_width = width;
        return (1);
    }
    return (0);
}