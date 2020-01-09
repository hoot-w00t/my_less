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

void init_screen(void)
{
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(FALSE);
}

void display_lines(lessfile_t *lf)
{
    unsigned int height = 0, width = 0;
    unsigned int line = 0;
    unsigned int g = 0;
    getmaxyx(stdscr, height, width);

    while (line != lf->line) {
        if (lf->content[g] == '\n')
            ++line;
        ++g;
    }

    clear();
    for (unsigned int y = 0; y < height && lf->content[g] != '\0'; ++y, ++g) {
        for (unsigned int x = 0; x < width && !eol(lf->content[g]); ++x, ++g)
            mvwaddch(stdscr, y, x, lf->content[g]);
    }
    refresh();
}

int handle_input(lessfile_t *lf, int *ch,
    unsigned int *height, unsigned int *width)
{
    unsigned int oheight = 0;
    unsigned int owidth = 0;
    getmaxyx(stdscr, oheight, owidth);

    if (*ch == KEY_DOWN && lf->line + 1 < lf->total_lines) {
        lf->line += 1;
        return (1);
    } else if (*ch == KEY_UP && lf->line > 0) {
        lf->line -= 1;
        return (1);
    }
    if (oheight != *height || owidth != *width) {
        *height = oheight;
        *width = owidth;
        return (1);
    }
    return (0);
}

void display_file(lessfile_t *lf)
{
    int ch = 0;
    unsigned int height, width;

    init_screen();
    getmaxyx(stdscr, height, width);
    display_lines(lf);

    while (ch != 'q') {
        ch = getch();
        if (handle_input(lf, &ch, &height, &width))
            display_lines(lf);
    }

    endwin();
}