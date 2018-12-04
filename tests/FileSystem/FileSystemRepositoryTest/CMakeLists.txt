set(FileSystemRepositoryTest_SRCS
    FileSystemRepositoryTest.cpp
)

configure_simplicity_test(FileSystemRepositoryTest "${FileSystemRepositoryTest_SRCS}" SimplicityFileSystem SimplicityUtil_StandardCppOnly)
