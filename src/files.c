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
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <libgen.h>

char *read_file(char *filepath)
{
    struct stat _fileinfo;
    FILE *_file = NULL;
    char *file_content = NULL;

    if (lstat(filepath, &_fileinfo) == -1) {
        fprintf(stderr, "%s: %s\n", filepath, strerror(errno));
        return (NULL);
    }

    if (S_ISDIR(_fileinfo.st_mode)) {
        fprintf(stderr, "%s: Is a directory\n", filepath);
        return (NULL);
    }

    _file = fopen(filepath, "r");
    if (_file == NULL) {
        fprintf(stderr, "%s: %s\n", filepath, strerror(errno));
        return (NULL);
    }

    file_content = malloc(sizeof(char) * (_fileinfo.st_size + 1));
    if (file_content == NULL) {
        fprintf(stderr, "%s: Cannot allocate memory (read_file)\n", filepath);
        fclose(_file);
        return (NULL);
    }

    long read_bytes = 0;
    while (read_bytes != _fileinfo.st_size) {
        read_bytes += fread(&file_content[read_bytes],
                            sizeof(char),
                            _fileinfo.st_size - read_bytes,
                            _file
                            );
    }
    file_content[read_bytes] = '\0';

    fclose(_file);
    return (file_content);
}

char *get_next_line(char *str, char **endptr)
{
    size_t size = 0;
    for (; !eol(str[size]); ++size);

    char *line = malloc(sizeof(char) * (size + 1));
    if (line == NULL)
        return (NULL);

    strncpy(line, str, size);
    line[size] = '\0';
    if (str[size] == '\n') {
        *endptr = &str[size + 1];
    } else {
        *endptr = &str[size];
    }
    return (line);
}

int split_lines(char *str, lessfile_t *lf)
{
    size_t lines = 1;
    size_t line_len = 0;

    for (size_t i = 0; str[i] != '\0'; ++i)
        if (str[i] == '\n')
            ++lines;
    lf->line_c = lines;

    char **splitted = malloc(sizeof(char *) * lines);
    if (splitted == NULL)
        return (0);

    char *endptr = str;
    for (size_t i = 0; i < lines; ++i) {
        splitted[i] = get_next_line(endptr, &endptr);
        line_len = strlen(splitted[i]);

        if (line_len > lf->column_max)
            lf->column_max = line_len;
        if (splitted[i] == NULL)
            lines = i + 1;
    }

    lf->content = splitted;
    return (1);
}

lessfile_t *load_file(char *filepath)
{
    char *content = read_file(filepath);
    if (content == NULL)
        return (NULL);

    lessfile_t *lf = malloc(sizeof(lessfile_t));
    if (lf == NULL) {
        fprintf(stderr, "%s: Cannot allocate memory (load_file)\n", filepath);
        free(content);
        return (NULL);
    }

    char *_path = strdup(filepath);
    lf->filename = strdup(basename(_path));
    if (_path == NULL || lf->filename == NULL) {
        fprintf(stderr, "%s: Cannot allocate memory (filename)\n", filepath);
        if (_path != NULL)
            free(_path);
        if (lf->filename != NULL)
            free(lf->filename);
        free(content);
        free(lf);
        return (NULL);
    }

    lf->filepath = filepath;
    lf->line = 0;
    lf->column = 0;
    lf->column_max = 0;

    split_lines(content, lf);
    free(content);
    free(_path);

    return (lf);
}

void unload_file(lessfile_t *lf)
{
    for (size_t i = 0; i < lf->line_c; ++i)
        free(lf->content[i]);
    free(lf->filename);
    free(lf->content);
    free(lf);
}