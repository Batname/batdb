#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

void fatal(char *message)
{
    printf("Exit error with message: [%s]\n", message);
    exit(-1);
}

void usage(char *prog_name, char *filename)
{
    printf("Usage: %s <data to add to %s>\n", prog_name, filename);
    exit(0);
}