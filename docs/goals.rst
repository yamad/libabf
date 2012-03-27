`libabf` is an open-source, portable C library for reading and writing
files in Axon Binary Format (ABF).

Background
==========

The ABF format is a binary data format developed by Axon Instruments
(recently acquired by Molecular Devices), and is used by their
electrophysiology data acquisition software. Axon provides file
support for the Windows OS only in the form of a dynamic link library
(*.dll).

As of March 2012, there are two versions of the ABF format--ABF1 and
ABF2. ABF2 improves on ABF1 by allowing for a dynamically sized header
region.

A file support pack provided by Axon contains C++ source code that
provides rudimentary support for reading ABF files. The code uses the
Windows API extensively and makes assumptions about struct packing and
type sizes that are only appropriate for Windows.

Goals
=====

`libabf` is a library providing read access to ABF files. In the
future, it may support all Axon file formats, including the text
format ATF.

`libabf` is:

 * open-source
 * standard C89
 * portable/cross-platform

Portability
===========

`libabf` aims to support any compiler supporting standard C89, subject
to the following assumptions about the host compiler and architecture:

 * 8-, 16-, 32-, and 64-bit type sizes are exactly representable
 * IEEE-754 standard floating-point values are supported

These assumptions are not required by the C standard. However, almost
all modern systems on which `libabf` is conceivably useful will
fulfill these requirements.

Features of C99, such as exact-width integer types (```stdint.h```)
and the boolean type (```stdbool.h```), are backported to standard C89
when they are not available natively.

Useful features of specific operating systems not included in the C
standard may be used to enhance performance. However, all user-facing
features must be supported in standard C89. For instance,
memory-mapped files may be used on systems that support them, but
systems that do not have memory-mapped files will still be able to use
`libabf`.

Structure packing
-----------------

To remain optimally portable, `libabf` makes no assumptions about
structure packing, data alignment, byte order, or type sizes. Safe
reading and writing to the appropriate data layout must be done
through the serialization and deserialization routines.

`libabf`-defined structures are more portable versions of the
structures defined in the header files supplied by Axon
Instruments. The `libabf` structures are explicit about the expected
size of each data member, and the order of data members is preserved.

Portable C code cannot guarantee that the memory layout of the
structures is the same as the data layout in the file itself. However,
all other ABF reading/writing code this author has seen to date
depends on the non-standard :inline:`#pragma pack` directive
(supported by Visual Studio and gcc). The data layout of a structure
as stored in an ABF file is the memory layout of the structure when
the :inline:`#pragma pack(1)` option is active.

Endian
------

ABF files are written in the native byte order of the computer that
wrote the file. The byte order of the file can be determined from the
order of the file signature, which is stored in the first four bytes.

Other software
==============

Some other open-source projects that have developed support for ABF
files include:

stimfit_

    A electrophysiology analysis suite developed by Christoph
    Schmidt-Hieber written in C++ with a Python shell. Contains
    support for opening various electrophysiology data files. Support
    for ABF files is read-only, and is integrated directly into its
    file I/O system. Not all struct data is available. Stimfit
    includes the C++ code released by Axon and supports non-Windows
    systems using a platform-specific header file that translates
    Windows-specific types.
    
QuB_

    QuB is a single-channel data analysis suite developed at the
    University of Buffalo. QuB is written in Delphi, a
    Windows-specific language.

libaxon_

   An open-source C++ library for reading ABF2 files. At the time of
   writing, this library successfully read file headers but crashed
   when accessing data. Also, libaxon uses non-standard compiler
   extensions to force struct packing as Axon expects it. Nonetheless,
   the goals of this project are closely-aligned with the goals of
   `libabf`.

.. _stimfit: http://www.stimfit.org
.. _QuB: http://www.qub.buffalo.edu
.. _libaxon: http://sourceforge.net/projects/libaxon
