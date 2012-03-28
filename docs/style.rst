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

If the type represents a struct, the struct tag should be the same as
the type but without the ```*_dt``` suffix. For a example::

    typedef struct foo foo_dt;
    typedef uint8_t byte_dt;

In favor of explicitness, following the reasoning of the Linux Kernel
coding style, typedefs are generally only used for truly opaque
types. Otherwise, a simple struct definition suffices. Pointers shall
not be hidden by typedefs.


enums
=====

Enumeration types are in CamelCase. Enumeration members are also in
CamelCase and are prefixed with the enum tag separated by an
underscore. If defined, which is recommended, an enum typedef should
be identical to the enum tag.

For example::

   typedef enum Foo {
       Foo_Bar,
       Foo_BazMoo
   } Foo;

enums are the only construct named in CamelCase. Because they act as
constants, one could argue that an uppercase naming convention is
appropriate. However, enum member names can become long and CamelCase
is less "noisy" and more readable for such long names.
