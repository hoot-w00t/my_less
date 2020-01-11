#include "version.h"
#include <stdio.h>

void display_version(void)
{
    printf("binary built from commit: %s\n", GIT_VERSION);
}
