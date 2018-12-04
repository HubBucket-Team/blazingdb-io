set(UriTest_SRCS
    UriTest.cpp
)

configure_simplicity_test(UriTest "${UriTest_SRCS}" SimplicityFileSystem SimplicityUtil_StandardCppOnly)
