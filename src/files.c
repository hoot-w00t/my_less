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
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

char *read_file(char *filepath)
{
    struct stat _fileinfo;
    FILE *_file = NULL;
    char *file_content = NULL;

    if (lstat(filepath, &_fileinfo) == -1) {
        fprintf(stderr, "%s: %s\n", filepath, strerror(errno));
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

    unsigned int read_bytes = 0;
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

    lf->line = 0;
    lf->total_lines = 1;
    lf->content = content;
    for (unsigned int i = 0; content[i] != '\0'; ++i) {
        if (content[i] == '\n')
            lf->total_lines += 1;
    }

    return (lf);
}

void unload_file(lessfile_t *lf)
{
    free(lf->content);
    free(lf);
}