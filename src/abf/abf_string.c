#include <stdlib.h>
#include "abf_string.h"

#define ABF_STRCACHE_BLOCK_SIZE 2

int abf_strcache_init(struct abf_strcache *sc) {
    sc->count = 0;
    sc->cache_size = 0;
    sc->strings = NULL;
    abf_strcache_extend(sc, ABF_STRCACHE_BLOCK_SIZE);
    if (NULL == sc->strings)
        return 1;
    return 0;
}

int abf_strcache_add(struct abf_strcache *sc, char *str) {
    if (sc->count == sc->cache_size)
        abf_strcache_extend(sc, ABF_STRCACHE_BLOCK_SIZE);
    sc->strings[sc->count++] = str;
}

int abf_strcache_extend(struct abf_strcache *sc, size_t nentries) {
    int new_size = sc->cache_size + nentries;
    char **tmp = realloc(sc->strings, sizeof(sc->strings) * new_size);
    if (NULL == tmp)
        /* reallocation failed */
        return 1;
    sc->strings = tmp;
    sc->cache_size = new_size;
    return 0;
}
