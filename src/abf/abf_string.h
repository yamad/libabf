#ifndef _ABF_STRING_H_
#define _ABF_STRING_H_

#include <stdint.h>
#include <string.h>

struct abf_strcache {
    size_t count;               /* number of strings */
    char **strings;             /* pointer to strings */
    size_t cache_size;          /* number of entries in cache */
};

int abf_strcache_init(struct abf_strcache *sc);
int abf_strcache_destroy(struct abf_strcache *sc);

/* return the `n`-th string in the cache */
char* abf_strcache_get(struct abf_strcache *sc, size_t n);

/* add copy of string `str` to the string cache */
int abf_strcache_add(struct abf_strcache *sc, char *str);

/* remove `n`-th string in the cache
 *
 * in its current implementation, removing a string is expensive and
 * should generally not be done.
 */
int abf_strcache_remove(struct abf_strcache *sc, size_t n);

/* extend the cache size by `nentries` entries */
int abf_strcache_extend(struct abf_strcache *sc, size_t nentries);



#endif /* _ABF_STRING_H_ */
