#=============================================================================
# Copyright 2018 BlazingDB, Inc.
#     Copyright 2018 Felipe Aramburu <felipe@blazingdb.com>
#     Copyright 2018 Alexander Ocsa <alexander@blazingdb.com>
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

# Usage:
#    $ mkdir build       # create directory for out-of-source build
#    $ cd build          # enter the build directory
#    $ cmake ..          # configure build system
#    $ make              # make libgdf
#    $ make pytest       # trigger test
#    $ make install      # install libgdf

PROJECT(blazing-io)

cmake_minimum_required(VERSION 3.11)  # not sure about version required

set(CMAKE_CXX_STANDARD 11)
message(STATUS "Using C++ standard: c++${CMAKE_CXX_STANDARD}")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/" ${CMAKE_MODULE_PATH})
message(STATUS "CMAKE_MODULE_PATH:" "${CMAKE_MODULE_PATH}")


# Include CMake modules
include(FeatureSummary)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CTest)
include(ConfigureAWSSDK) 

# Include custom modules (see cmake directory)
include(ConfigureArrow)
include(ConfigureParquetCpp)

# Locate the AWS SDK for C++ package.
# Requires that you build with:
#   -Daws-sdk-cpp_DIR=/path/to/sdk_build
# or export/set:
#   CMAKE_PREFIX_PATH=/path/to/sdk_build
message(STATUS "aws-sdk-cpp_DIR: " ${aws-sdk-cpp_DIR})
find_package(aws-sdk-cpp REQUIRED)
set_package_properties(aws-sdk-cpp PROPERTIES TYPE REQUIRED PURPOSE "AWS SDK for C++ allows to integrate any C++ application with AWS services." URL "https://aws.amazon.com/sdk-for-cpp/")
configure_aws_sdk()

# Locate the Apache Arrow package (Requires that you use ConfigureArrow module)
message(STATUS "ARROW_ROOT: " ${ARROW_ROOT})
find_package(Arrow REQUIRED)
set_package_properties(Arrow PROPERTIES TYPE REQUIRED
    PURPOSE "Apache Arrow is a cross-language development platform for in-memory data."
    URL "https://github.com/apache/arrow")

if (ARROW_FOUND)
    message(STATUS "Apache Arrow found in ${ARROW_ROOT}")
else()
    message(FATAL_ERROR "Apache Arrow not found, please check your settings.")
endif()

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${ARROW_INCLUDEDIR}"
)

#find_package(AWSSDK REQUIRED COMPONENTS aws-cpp-sdk-core aws-cpp-sdk-s3 aws-cpp-sdk-s3-encryption aws-cpp-sdk-kms)

add_library(blazing-io-lib STATIC  
    src/Util/StringUtil.cpp
    src/Util/EncryptionUtil.cpp
    src/Util/FileUtil.cpp
    src/Config/BlazingContext.cpp
    src/ExceptionHandling/BlazingThread.cpp
    src/ExceptionHandling/BlazingException.cpp
    src/ExceptionHandling/BlazingExceptionHolder.cpp
    src/Library/Logging/BlazingLogger.cpp
    src/Library/Logging/CoutOutput.cpp
    src/Library/Logging/Logger.cpp
    src/Library/Logging/LoggingLevel.cpp
    src/Library/Logging/ServiceLogging.cpp
    src/Library/Logging/TcpOutput.cpp
    src/FileSystem/FileSystemType.cpp
    src/FileSystem/Path.cpp
    src/FileSystem/Uri.cpp
    src/FileSystem/FileStatus.cpp
    src/FileSystem/FileFilter.cpp
    src/FileSystem/FileSystemConnection.cpp
    src/FileSystem/FileSystemException.cpp
    src/FileSystem/LocalFileSystem.cpp
    src/FileSystem/HadoopFileSystem.cpp
    src/FileSystem/S3FileSystem.cpp
    src/FileSystem/FileSystemManager.cpp
    src/FileSystem/FileSystemEntity.cpp
    src/FileSystem/FileSystemRepository.cpp
    src/FileSystem/FileSystemCommandParser.cpp
    src/FileSystem/private/S3ReadableFile.cpp
    src/FileSystem/private/S3OutputStream.cpp
    src/FileSystem/private/LocalFileSystem_p.cpp
    src/FileSystem/private/HadoopFileSystem_p.cpp
    src/FileSystem/private/S3FileSystem_p.cpp
    src/FileSystem/private/FileSystemManager_p.cpp
    src/FileSystem/private/FileSystemFactory.cpp
    src/FileSystem/private/FileSystemRepository_p.cpp
  )

include_directories(blazing-io-lib ${CMAKE_CURRENT_SOURCE_DIR}/src)
# include_directories(blazing-io-lib /home/felipe/cuda-workspace/blazing-io/build/CMakeFiles/thirdparty/arrow-download/arrow-prefix/src/arrow/cpp/src)


#include_directories(blazing-io-run ${CMAKE_CURRENT_SOURCE_DIR}/src)

if (ARROW_NUMERIC_VERSION EQUAL 900)
#   arrow-0.9 requires boost_regex
	target_link_libraries(blazing-io-lib Apache::ParquetCpp arrow boost regex aws-cpp-sdk-s3 aws-cpp-sdk-s3-encryption)
#	target_link_libraries(blazing-io-run Apache::ParquetCpp arrow boost regex ${CUDA_CUDA_LIB} aws-cpp-sdk-s3 aws-cpp-sdk-s3-encryption libgdf.a)
else()
	target_link_libraries(blazing-io-lib Apache::ParquetCpp arrow aws-cpp-sdk-s3 aws-cpp-sdk-s3-encryption)
#	target_link_libraries(blazing-io-run Apache::ParquetCpp arrow ${CUDA_CUDA_LIB}  aws-cpp-sdk-s3 aws-cpp-sdk-s3-encryption libgdf.a)
endif()

# Command to symlink files into the build directory
add_custom_command(  # link the include directory
    OUTPUT include
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/include include
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/include)

# The install target
install(TARGETS blazing-io-lib
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        COMPONENT library
        )

install(DIRECTORY src/Config DESTINATION include)
install(DIRECTORY src/ExceptionHandling DESTINATION include)
install(DIRECTORY src/FileReaders DESTINATION include)
install(DIRECTORY src/FileSystem DESTINATION include)
install(DIRECTORY src/Library DESTINATION include)
install(DIRECTORY src/Util DESTINATION include)

# Print the project summary
feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
