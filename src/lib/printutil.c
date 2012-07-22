#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "printutil.h"

char *get_repeated_string(char c, int repeats) {
    char *dest = calloc(repeats + 1, sizeof(char));
    if (dest == NULL) {
        printf("Memory failure.");
        exit(1);
    }
    if (repeats > 0) {
        memset(dest, c, repeats);
        *(dest + repeats) = '\0';
    }
    return dest;
}
