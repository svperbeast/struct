cmake_minimum_required (VERSION 2.8)

project (struct)

set (gtest "gtest-1.6.0")

# remove -rdynamic
set (CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
set (CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)

add_subdirectory (${gtest})

include_directories ("${struct_SOURCE_DIR}/include"
                     "${gtest_SOURCE_DIR}/include"
                     )

link_directories ("${CMAKE_BINARY_DIR}/lib"
                  "${gtest_BINARY_DIR}"
                  )

add_library (struct SHARED
             src/endian.c
             src/struct.c
             )

set_target_properties (struct PROPERTIES
                       LIBRARY_OUTPUT_DIRECTORY
                       "${CMAKE_BINARY_DIR}/lib"
                       COMPILE_FLAGS
                       "${CMAKE_C_FLAGS} -O2 -Wall -W"
                       )

install (TARGETS struct DESTINATION "${CMAKE_BINARY_DIR}/export/lib")
install (FILES
         "${struct_SOURCE_DIR}/include/struct.h"
         DESTINATION
         "${CMAKE_BINARY_DIR}/export/include"
         )

include (CTest)
enable_testing ()

set (MEMORYCHECK_COMMAND /usr/bin/valgrind)
set (MEMORYCHECK_COMMAND_OPTIONS "--tool=memcheck --leack-check=yes")

add_executable (struct_test
                test/struct_test.cpp
                )

target_link_libraries (struct_test struct gtest)

set_target_properties (struct_test PROPERTIES
                       RUNTIME_OUTPUT_DIRECTORY
                       "${CMAKE_BINARY_DIR}"
                       )

add_test (StructTest "${CMAKE_BINARY_DIR}/struct_test")
