#=============================================================================
# Copyright 2018 BlazingDB, Inc.
#     Copyright 2018 Felipe Aramburu <felipe@blazingdb.com>
#     Copyright 2018 Percy Camilo Triveño Aucahuasi <percy@blazingdb.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#=============================================================================

PROJECT(blazingdb-io)

cmake_minimum_required(VERSION 3.11)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/" ${CMAKE_MODULE_PATH})
message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

# NOTE Import cmake modules from blazingdb-dependencies (Mandatory: First step!)
include(ImportCMakeModules)
include(ConfigureBlazingDBHostCompiler)

# Include CMake modules
include(FeatureSummary)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CTest)

# NOTE Configure dependencies (always include this one first)
include(ConfigureBlazingDBDependencies)

# NOTE Do not change the inclusion order
# Configure dependencies
# TODO percy c.gonzales use native hdfs lib instead of arrow wrapper
#include(Configure pivotalhdfs)
include(ConfigureArrow)
include(ConfigureAWSSDK)

set(FILESYSTEM_SRC_FILES
    ${CMAKE_SOURCE_DIR}/src/FileSystem/FileSystemType.cpp
    ${CMAKE_SOURCE_DIR}/src/FileSystem/Path.cpp
    ${CMAKE_SOURCE_DIR}/src/FileSystem/Uri.cpp
    ${CMAKE_SOURCE_DIR}/src/FileSystem/FileStatus.cpp
    ${CMAKE_SOURCE_DIR}/src/FileSystem/FileFilter.cpp
    ${CMAKE_SOURCE_DIR}/src/FileSystem/FileSystemConnection.cpp
    ${CMAKE_SOURCE_DIR}/src/FileSystem/FileSystemException.cpp
    ${CMAKE_SOURCE_DIR}/src/FileSystem/LocalFileSystem.cpp
    ${CMAKE_SOURCE_DIR}/src/FileSystem/HadoopFileSystem.cpp
    ${CMAKE_SOURCE_DIR}/src/FileSystem/S3FileSystem.cpp
    ${CMAKE_SOURCE_DIR}/src/FileSystem/FileSystemManager.cpp
    ${CMAKE_SOURCE_DIR}/src/FileSystem/FileSystemEntity.cpp
    ${CMAKE_SOURCE_DIR}/src/FileSystem/FileSystemRepository.cpp
    ${CMAKE_SOURCE_DIR}/src/FileSystem/FileSystemCommandParser.cpp
    ${CMAKE_SOURCE_DIR}/src/FileSystem/private/S3ReadableFile.cpp
    ${CMAKE_SOURCE_DIR}/src/FileSystem/private/S3OutputStream.cpp
    ${CMAKE_SOURCE_DIR}/src/FileSystem/private/LocalFileSystem_p.cpp
    ${CMAKE_SOURCE_DIR}/src/FileSystem/private/HadoopFileSystem_p.cpp
    ${CMAKE_SOURCE_DIR}/src/FileSystem/private/S3FileSystem_p.cpp
    ${CMAKE_SOURCE_DIR}/src/FileSystem/private/FileSystemManager_p.cpp
    ${CMAKE_SOURCE_DIR}/src/FileSystem/private/FileSystemFactory.cpp
    ${CMAKE_SOURCE_DIR}/src/FileSystem/private/FileSystemRepository_p.cpp)

set(LOGGING_SRC_FILES
    ${CMAKE_SOURCE_DIR}/src/Library/Logging/BlazingLogger.cpp
    ${CMAKE_SOURCE_DIR}/src/Library/Logging/CoutOutput.cpp
    ${CMAKE_SOURCE_DIR}/src/Library/Logging/FileOutput.cpp
    ${CMAKE_SOURCE_DIR}/src/Library/Logging/Logger.cpp
    ${CMAKE_SOURCE_DIR}/src/Library/Logging/LoggingLevel.cpp
    ${CMAKE_SOURCE_DIR}/src/Library/Logging/ServiceLogging.cpp
    ${CMAKE_SOURCE_DIR}/src/Library/Logging/TcpOutput.cpp)

set(EXCEPTION_SRC_FILES
    ${CMAKE_SOURCE_DIR}/src/ExceptionHandling/BlazingThread.cpp
    ${CMAKE_SOURCE_DIR}/src/ExceptionHandling/BlazingException.cpp
    ${CMAKE_SOURCE_DIR}/src/ExceptionHandling/BlazingExceptionHolder.cpp)

set(UTIL_SRC_FILES
    ${CMAKE_SOURCE_DIR}/src/Util/StringUtil.cpp
    ${CMAKE_SOURCE_DIR}/src/Util/EncryptionUtil.cpp
    ${CMAKE_SOURCE_DIR}/src/Util/FileUtil.cpp
    ${CMAKE_SOURCE_DIR}/src/Config/BlazingContext.cpp)

add_library(blazingdb-io STATIC
    ${FILESYSTEM_SRC_FILES}
    ${UTIL_SRC_FILES}
    ${LOGGING_SRC_FILES}
    ${EXCEPTION_SRC_FILES}
)

include_directories(blazingdb-io ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(blazingdb-io ${ARROW_STATIC_LIB} aws-cpp-sdk-s3 aws-cpp-sdk-s3-encryption)

# Install target and headers
install(TARGETS blazingdb-io DESTINATION lib)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/ DESTINATION include/blazingdb/io FILES_MATCHING PATTERN "*.h")

# Tests
if(BUILD_TESTING)
    include(ConfigureGoogleTest)

    if(GTEST_FOUND)
        add_subdirectory(tests)
    else()
        message(AUTHOR_WARNING "Google C++ Testing Framework (Google Test) not found: automated tests are disabled.")
    endif()
endif()

# Print the project summary
feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
