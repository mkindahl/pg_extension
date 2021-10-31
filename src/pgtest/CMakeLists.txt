# 1. Read pgxs makefile and find the source directory.
# 2. Read the objfiles.txt for src/backend/nodes

set(top_srcdir /usr/local/src/postgresql-13.2)
set(subdirs access bootstrap catalog parser commands executor foreign lib libpq 
  main nodes optimizer partitioning port postmaster 
  regex replication rewrite 
  statistics storage tcop tsearch utils
  jit)
list(TRANSFORM subdirs PREPEND "${top_srcdir}/src/backend/")
list(APPEND subdirs ${top_srcdir}/src/timezone)
set(objfiles)
foreach(subdir ${subdirs})
  file(READ "${subdir}/objfiles.txt" _contents)
  string(REGEX REPLACE "[ \t\r\n]+" ";" _objfiles "${_contents}")
  list(TRANSFORM _objfiles PREPEND "${top_srcdir}/")
  list(TRANSFORM _objfiles REPLACE ".o[ \t\n]*$" ".c")
  list(APPEND objfiles ${_objfiles})
endforeach()
add_library(pgtest STATIC ${objfiles})
target_compile_definitions(pgtest PRIVATE
  DLSUFFIX="${CMAKE_SHARED_LIBRARY_SUFFIX}"
  _GNU_SOURCE=1)

