set(FileSystemCommandParserTest_SRCS
    ParseRegisterFileSystemTest.cpp
    ParseDeregisterFileSystemTest.cpp
)

configure_simplicity_test(FileSystemCommandParserTest "${FileSystemCommandParserTest_SRCS}" SimplicityFileSystem SimplicityUtil_StandardCppOnly)
