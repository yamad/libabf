#include <stdlib.h>
#include "abf_string.h"

#define ABF_STRCACHE_BLOCK_SIZE 10

int abf_strcache_init(struct abf_strcache *sc) {
    sc->count = 0;
    sc->cache_size = 0;
    sc->strings = NULL;
    abf_strcache_extend(sc, ABF_STRCACHE_BLOCK_SIZE);
    if (NULL == sc->strings)
        return 1;
    return 0;
}

int abf_strcache_destroy(struct abf_strcache *sc) {
    int i;
    for (i=0; i < sc->count; i++) {
        free(*(sc->strings+i));
    }
    free(sc->strings);
}

int abf_strcache_add(struct abf_strcache *sc, char *str) {
    size_t len = strlen(str);
    char *dest = malloc(len+1);
    strcpy(dest, str);
    if (sc->count == sc->cache_size)
        abf_strcache_extend(sc, ABF_STRCACHE_BLOCK_SIZE);
    sc->strings[sc->count++] = dest;
}

char *abf_strcache_get(struct abf_strcache *sc, size_t n) {
    if (n < sc->count)
        return sc->strings[n];
    return NULL;
}

int abf_strcache_remove(struct abf_strcache *sc, size_t n) {
    int i;
    if (n >= sc->count)         /* out of range */
        return 1;

    free(*(sc->strings+n));
    for (i=n+1; i < sc->count; i++)
        *(sc->strings+(i-1)) = *(sc->strings+i);
    sc->strings[i-1] = NULL;
    sc->count--;
    return 0;
}

int abf_strcache_extend(struct abf_strcache *sc, size_t nentries) {
    int new_size = sc->cache_size + nentries;

    char **tmp = realloc(sc->strings, sizeof(char*) * new_size);
    if (NULL == tmp)
        return 1;

    size_t i;                   /* set new pointers to null */
    for (i = sc->cache_size; i < new_size; i++) {
        *(tmp+i) = NULL;
    }

    sc->strings = tmp;
    sc->cache_size = new_size;
    return 0;
}

int abf_strcache_printall(struct abf_strcache *sc, int indent) {
    int i;
    for (i=0; i < sc->count; i++) {
        printf("[%2.2d] %s\n", i, abf_strcache_get(sc, i));
    }
}
