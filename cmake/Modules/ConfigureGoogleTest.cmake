#=============================================================================
# Copyright 2018 BlazingDB, Inc.
#     Copyright 2018 Percy Camilo Triveño Aucahuasi <percy@blazingdb.com>
#=============================================================================

# BEGIN macros

macro(CONFIGURE_GOOGLE_TEST_EXTERNAL_PROJECT)
    set(GOOGLE_TEST_CMAKE_ARGS
        " -Dgtest_build_samples=ON"
        " -DCMAKE_C_FLAGS=-D_GLIBCXX_USE_CXX11_ABI=0"      # enable old ABI for C/C++
        " -DCMAKE_CXX_FLAGS=-D_GLIBCXX_USE_CXX11_ABI=0")   # enable old ABI for C/C++

    # Download and unpack googletest at configure time
    configure_file(${CMAKE_SOURCE_DIR}/cmake/Templates/GoogleTest.CMakeLists.txt.cmake ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/thirdparty/googletest-download/CMakeLists.txt)

    execute_process(
        COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/thirdparty/googletest-download/
    )

    if(result)
        message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()

    execute_process(
        COMMAND ${CMAKE_COMMAND} --build . -- -j8
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/thirdparty/googletest-download/
    )

    if(result)
        message(FATAL_ERROR "Build step for googletest failed: ${result}")
    endif()
endmacro()

# END macros

# BEGIN MAIN #

if (GOOGLE_TEST_INSTALL_DIR)
    message(STATUS "GOOGLE_TEST_INSTALL_DIR defined, it will use vendor version from ${GOOGLE_TEST_INSTALL_DIR}")
    set(GTEST_ROOT "${GOOGLE_TEST_INSTALL_DIR}")
else()
    message(STATUS "GOOGLE_TEST_INSTALL_DIR not defined, it will be built from sources")
    configure_google_test_external_project()
    set(GTEST_ROOT "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/thirdparty/googletest-install/")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

message(STATUS "GTEST_ROOT: " ${GTEST_ROOT})

find_package(GTest QUIET)
set_package_properties(GTest PROPERTIES TYPE OPTIONAL
    PURPOSE "Google C++ Testing Framework (Google Test)."
    URL "https://github.com/google/googletest")

link_directories(${GTEST_ROOT}/lib/)
include_directories(${GTEST_INCLUDE_DIRS})

if(GTEST_FOUND)
    message(STATUS "Google C++ Testing Framework (Google Test) found in ${GTEST_ROOT}")
else()
    message(AUTHOR_WARNING "Google C++ Testing Framework (Google Test) not found: automated tests are disabled.")
    set(BUILD_TESTING OFF)
endif()

# END MAIN #
