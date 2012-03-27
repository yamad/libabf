====================
 Coding Conventions
====================

typedefs
========

Defined types are all lowercase and end with the suffix ```*_dt```,
which stands for *data type*. This scheme is a variation of the
```*_t``` convention for naming types in Standard C and POSIX. The
```*_t``` suffix cannot be used directly because POSIX reserves it for
implementations.

If the type represents a struct, the strut tag should be the same as
the type but without the ```*_dt``` suffix. For a example::

    typedef struct foo foo_dt;
    typedef uint8_t byte_dt;

In favor of explicitness, following the reasoning of the Linux Kernel
coding style, typedefs are generally only used for truly opaque
types. Otherwise, a simple struct definition suffices. Pointers shall
not be hidden by typedefs.


enums
=====

Enumeration members are all uppercase and prefixed with the name of
the enumeration type. The enumeration tag is lowercase. If defined (as
recommended), the typedef is all uppercase. For example::

   typedef enum bar {
       BAR_A,
       BAR_B
   } BAR;
