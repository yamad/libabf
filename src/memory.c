#ifdef __cplusplus
#define NEW(type) ((type *) malloc(sizeof(type)))
#else
#define NEW(type) (malloc(sizeof(type)))
#endif

#define FREE(pointer) \
    {                 \
    free(pointer);    \
    pointer = NULL;   \
    }
