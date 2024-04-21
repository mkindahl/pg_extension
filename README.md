# CMake-based PostgreSQL Extensions

Template project for building CMake-based PostgreSQL extensions.

The project is the base for creating PostgreSQL extensions using CMake
rather than the supported PGXS scripts. The macros distributed with
this project use `pg_config` to figure out what settings are
necessary.

CMake has a few advantages compared to vanilla Make:

1. It is well-supported by various IDEs on differen platforms.
2. It supports cross-platform file manipulations.
3. It has cross-platform support for configuration files, allowing you
   to generate files from templates.
4. It has cross-platform support for handling shared libraries, static
   libraries, and executables.
5. It has cross-platform support for creating packages.

## Dependencies

To build the extensions you need to have:
- CMake version 3.10 or later
- PostgreSQL server version 10 or later.
- PostgreSQL server development files version 10 or later.

You can install these dependencies on Ubuntu (18.04 or later) using:

```
apt install cmake postgresql postgresql-server-dev-all
```

## Building

To build the extensions for installation using the default prefix
(`/usr/local`) and keep the build files separate from the source
files:

```bash
cmake -S . -B build
cmake --build build
```

If you want to use a different installation prefix than the default,
you can set `PGPATH` to point to the prefix where you have installed
PostgreSQL. For example, `/usr/local` if you have a local installation
that used this prefix, or as in my case, when I have several versions
installed in parallel, I could use `/usr/local/pgsql/11.7` like this:

```bash
cmake -S . -B build -DPGPATH=/usr/local/pgsql/16.2
cmake --build ..
```

## Creating Extensions

The macro `add_postgresql_extension` can be used to create
extensions. Here is a minimal top-level `CMakeLists.txt` file that
creates a single extension.

```
cmake_minimum_required(VERSION 3.10)
project(my_package)

# Make sure FindPostgreSQL.cmake is in the module path.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(PostgreSQL REQUIRED)

add_postgresql_extension(magic
  VERSION 1.0
  SOURCES magic.c more_magic.c
  SCRIPTS magic--1.0.sql
  TESTS magic_puff magic_smoke)
```

The macro `add_postgresql_extension` takes a name of the extension,
and several different options:

* `VERSION` is the version of the package and is required.
* `SOURCES` is a list of source files for the package. At least one
  source file need to be provided.
* `SCRIPTS` are the upgrade and install scripts for the extension. At
  least one install script for the version need to be added, but you
  can have several different combinations of install and upgrade
  scripts.
* `TESTS` is a list of regression tests. The test scripts are stored
  in the `sql/` directory, and the expected output of the scripts are
  stored in the `expected/` directory.
* `ENCODING` gives the encoding of the script files and is used when
  generating the control file.
* `SCRIPT_TEMPLATES` is an alternative to use for generating script
  files. The listed script template files will be used to generate
  scripts files by expanding variables in the template files.
* `REQUIRES` is a list of dependencies of the extension on other
  extensions and is used when generating the control file.
  
The macro will create a library for installation with the appropriate
name (for example, `magic.so`) and a control file for the extension
will be automatically generated (named `magic.control`).

If any tests are given using `TESTS`, a test target to run the test
will be added as well as a custom target to update the file with the
expected output.

## Having multiple extensions in the same repository

If you want to keep multiple extensions in the same repository, it is
usually easier to keep them in a separate `src` directory with one
directory for each extension. For example, here is a repository with
two extensions `column` and `trace` and a directory `common` with
shared code.

```
.
├── cmake
│   └── FindPostgreSQL.cmake
├── CMakeLists.txt
├── README.md
└── src
    ├── CMakeLists.txt
    ├── column
    │   ├── colam.c
    │   ├── colam.h
    │   ├── colam_handler.c
    │   └── Makefile
    ├── common
    │   ├── amutils.h
    │   ├── CMakeLists.txt
    │   ├── debug.c
    │   └── debug.h
    └── trace
        ├── CMakeLists.txt
        ├── expected
        │   ├── create.out
        │   ├── insert.out
        │   └── select.out
        ├── Makefile
        ├── sql
        │   ├── create.sql
        │   ├── insert.sql
        │   └── select.sql
        ├── traceam--0.1.sql
        ├── traceam.c
        ├── traceam.h
        └── traceam_handler.c

8 directories, 28 files
```

In this case, the top-level `CMakeLists.txt` look similar, but adds
the `src` subdirectory:

```cmake
cmake_minimum_required(VERSION 3.10)
project(tableam)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(PostgreSQL REQUIRED)

include_directories(${PostgreSQL_INCLUDE_DIRS}
                    ${PostgreSQL_SERVER_INCLUDE_DIRS})

add_subdirectory(src)
```

Inside the src directory, we then add the other extensions, but can
also add the `src` directory as an include directory, allowing the
different sub-packages reference each others:

```cmake
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(common)
add_subdirectory(trace)
```

## Extensions Available in the Repository

The repository contains a few basic examples right now that are
C-like, but the intention is to create a basis for writing extensions
in C++ with a focus on modern C++ (which right now means C++14 and
later).

* `quaternion`: An extension demonstrating the introduction of user
  defined types. It implements support for
  [*quaternions*][quaternions-wiki] that can be used to describe
  spatial rotation.

[quaternions-wiki]: https://en.wikipedia.org/wiki/Quaternion
