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

To maintain maximum portability, `libabf` aims to support any compiler
supporting standard C89.

An exception to standard C89 compatibility is that floating-point
values are assumed to be represented by IEEE-754 standard. However, I
don't know of any system that could make productive use of `libabf`
that does not use the IEEE-754 standard.

Useful features of C99, such as specific-width integer types and the
boolean type, are only used if backports in standard C89 are
available.

Useful features of specific operating systems not included in the C
standard may be used to enhance performance. However, all user-facing
features must be supported in standard C89. For instance,
memory-mapped files may be used on systems that support them, but
systems that do not have memory-mapped files will still be able to use
`libabf`.

Structure packing
-----------------

An important consequence of standard C compatibility is that the code
may make no assumptions about structure packing and type sizes.  This
assumption requires `libabf` to manually write and access structure
members, which is the only known way of avoiding structure packing and
endian issues in a portable way. This architecture adds complexity for
the sake of compatibility.

All other ABF code this author has seen to date depends on the
non-standard :inline:`#pragma pack` directive (supported by Visual
Studio and gcc).

Endian
------

ABF files are written in the native byte-order of the computer that
wrote the file. 

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
