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
/* add string `str` to the string cache */
int abf_strcache_add(struct abf_strcache *sc, char *str);


#endif /* _ABF_STRING_H_ */
