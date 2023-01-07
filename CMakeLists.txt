cmake_minimum_required (VERSION 3.1)

project (struct LANGUAGES C CXX)

set (CMAKE_C_STANDARD 99)
set (CMAKE_C_EXTENSIONS OFF)
set (CMAKE_C_STANDARD_REQUIRED ON)

#
# [Option(s)]
# STRUCT_BUILD_TEST: build googletest and test programs
# (e.g., cmake -DSTRUCT_BUILD_TEST=ON ..).
#

find_package (Threads REQUIRED)

option (STRUCT_BUILD_TEST "build googletest and test programs" OFF)

if (STRUCT_BUILD_TEST)
    include(ExternalProject)
    # googletest
    ExternalProject_Add (
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
        UPDATE_COMMAND ""
        INSTALL_COMMAND ""
        LOG_DOWNLOAD ON
        LOG_CONFIGURE ON
        LOG_BUILD ON
    )
    set_target_properties(googletest PROPERTIES EXCLUDE_FROM_ALL TRUE)
    ExternalProject_Get_Property (googletest source_dir)
    set (GTEST_INCLUDE_DIRS "${source_dir}/googletest/include")
    ExternalProject_Get_Property (googletest binary_dir)
    set (GTEST_LINK_DIRS "${binary_dir}/lib")


endif(STRUCT_BUILD_TEST)

# remove -rdynamic
set (CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
set (CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)

include_directories ("${struct_SOURCE_DIR}/include"
                     ${GTEST_INCLUDE_DIRS}
                     )

link_directories(${GTEST_LINK_DIRS})

add_library (struct
             src/struct_endian.c
             src/struct.c
             )

set_target_properties (struct PROPERTIES
		   COMPILE_FLAGS
		   "${CMAKE_C_FLAGS} -O2 -Wall"
		   )

install (TARGETS
           struct
         PERMISSIONS
           OWNER_READ OWNER_WRITE OWNER_EXECUTE
           GROUP_READ GROUP_WRITE GROUP_EXECUTE
           WORLD_READ WORLD_EXECUTE
         DESTINATION
           "${CMAKE_BINARY_DIR}/release/lib"
         EXPORT
           struct
        )

set (
  INCLUDE_INSTALL_DIR
    "${CMAKE_BINARY_DIR}/release/include/struct"
)

set (
  SRC_INCLUDE_SUPER_DIR
    "${struct_SOURCE_DIR}/include"
)

set (
  SRC_INCLUDE_DIR
    "${SRC_INCLUDE_SUPER_DIR}/struct"
)

install (FILES
         "${SRC_INCLUDE_DIR}/struct.h"
         DESTINATION
         "${INCLUDE_INSTALL_DIR}"
         )

target_include_directories (
  struct
  PUBLIC
  "$<BUILD_INTERFACE:${SRC_INCLUDE_SUPER_DIR};${SRC_INCLUDE_DIR}>"
  $<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>
)

export (
  TARGETS
    struct
  FILE
    "${CMAKE_BINARY_DIR}/struct-config.cmake"
)

export (
  PACKAGE
    struct
)

if (STRUCT_BUILD_TEST)
    include (CTest)
    enable_testing ()

    set (MEMORYCHECK_COMMAND valgrind)
    set (MEMORYCHECK_COMMAND_OPTIONS "--tool=memcheck --leack-check=yes")

    add_executable (struct_test
                    test/struct_test.cpp
                    )

    target_link_libraries (struct_test struct gtest)

    set_target_properties (struct_test PROPERTIES
                        RUNTIME_OUTPUT_DIRECTORY
                        "${CMAKE_BINARY_DIR}"
                        )
    add_dependencies(struct_test googletest)

    add_test (StructTest "${CMAKE_BINARY_DIR}/struct_test")
endif (STRUCT_BUILD_TEST)
