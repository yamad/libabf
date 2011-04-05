from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

ext = Extension(
    "ABF2",                             # name of extension
    ["ABF2.pyx", "axon.cpp"],           # source files
    language="c++",
    include_dirs=[],
    libraries=["stdc++",],
    cmdclass = {'build_ext': build_ext}
    )

setup(
    name = "ABF",
    ext_modules = ext
    )
