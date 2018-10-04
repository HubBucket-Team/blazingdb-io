macro(ADD_AWS_SDK_INCLUDE_DIR aws_target)
    get_target_property(aws_target_include_dir ${aws_target} INTERFACE_INCLUDE_DIRECTORIES)
    include_directories(${aws_target_include_dir})
endmacro()

macro(CONFIGURE_AWS_SDK)
    # Add the AWS SDK C++ includes: it seems AWS CMake files forgot to add the include_dir for each AWS lib.
    add_aws_sdk_include_dir(aws-cpp-sdk-core)
    add_aws_sdk_include_dir(aws-cpp-sdk-s3)
    add_aws_sdk_include_dir(aws-cpp-sdk-s3-encryption)
    add_aws_sdk_include_dir(aws-cpp-sdk-kms)

    message(STATUS "Using AWS SDK C++ module: AWS Core")
    message(STATUS "Using AWS SDK C++ module: AWS S3")
    message(STATUS "Using AWS SDK C++ module: AWS S3 Encryption")
endmacro()
