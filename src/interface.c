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
#include <stddef.h>
#include <ncurses.h>
#include <string.h>

void init_screen(void)
{
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(FALSE);
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_WHITE);
    }
}

void display_lines(lessfile_t *lf)
{
    unsigned int height = 0, width = 0, cur_line = lf->line;

    getmaxyx(stdscr, height, width);
    clear();
    for (unsigned int y = 0; y < height - 1; ++y, cur_line = lf->line + y) {
        if (cur_line < lf->line_c) {
            unsigned int line_len = strlen(lf->content[cur_line]);
            if (line_len < width) {
                mvaddnstr(y, 0, lf->content[cur_line], line_len);
            } else {
                mvaddnstr(y, 0, lf->content[cur_line], width);
            }
        } else {
            mvaddch(y, 0, '~');
        }
    }

    if (has_colors())
        attron(COLOR_PAIR(1));

    mvprintw(height - 1, 0, "%s, %u-%u (%u lines) (Press q to exit)",
                lf->filepath,
                lf->line + 1,
                lpool(lf->line + (height - 1), lf->line_c),
                lf->line_c);
    if (has_colors())
        attroff(COLOR_PAIR(1));

    refresh();
}

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

int handle_input(lessfile_t *lf, int *ch,
    unsigned int *prev_height, unsigned int *prev_width)
{
    unsigned int height = 0;
    unsigned int width = 0;

    getmaxyx(stdscr, height, width);
    switch (*ch) {
        case KEY_DOWN: return go_down(lf, 1);
        case KEY_PGDOWN: return go_down(lf, height);
        case KEY_UP: return go_up(lf, 1);
        case KEY_PGUP: return go_up(lf, height);
        case 'q': return (-1);
    }

    if (height != *prev_height || width != *prev_width) {
        *prev_height = height;
        *prev_width = width;
        return (1);
    }
    return (0);
}

void display_file(lessfile_t *lf)
{
    int ch = 0;
    unsigned int height, width;
    int r = 0;

    init_screen();
    getmaxyx(stdscr, height, width);
    display_lines(lf);

    while (r >= 0) {
        ch = getch();
        r = handle_input(lf, &ch, &height, &width);
        if (r > 0) {
            display_lines(lf);
        } else if (r < 0) {
            break;
        }
    }
    endwin();
}