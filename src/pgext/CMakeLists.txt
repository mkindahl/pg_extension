
# Extension for testing pgext functions
add_postgresql_extension(pgext
  VERSION 1.0
  SOURCES test_array.cpp test_scalar.cpp
  SCRIPTS pgext--1.0.sql
  TESTS array)

if(FALSE)
add_executable(test_function test_function.cpp)
add_executable(test_list test_list.cpp)
target_link_directories(test_list PRIVATE ${PostgreSQL_LIBRARY_DIRS})
target_link_options(test_list PRIVATE ${PostgreSQL_LINK_OPTIONS})
target_link_libraries(test_list GTest::GTest GTest::Main ${PostgreSQL_SERVER_LIBRARIES})

add_test(AllTests test_list)
endif()
