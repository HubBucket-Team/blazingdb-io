#include <iostream>
#include <limits.h>
#include <time.h>

#include "gtest/gtest.h"

#include "FileSystem/Path.h"

//NOTE overload ostream operator in order to print pretty messages in Google Test
//TODO decide if we want to move this function into Path.h
std::ostream & operator<< (std::ostream& os, const Path &path) {
	os << path.toString(false);
	return os;
}

//BEGIN test classes

class PathTest : public testing::TestWithParam<std::string> {
	protected:
		virtual void TearDown() {}

	protected:
		Path path;
};

class PathStringConstructorTest : public PathTest {
	protected:
		virtual void SetUp() {
			path = Path(GetParam(), true);
		}
};

class PathStringAssignmentTest : public PathTest {
	protected:
		virtual void SetUp() {
			path = GetParam();
		}
};

class ValidPathStringConstructorTest : public PathStringConstructorTest {};
class ValidPathStringAssignmentTest : public PathStringAssignmentTest {};

class InvalidPathStringConstructorTest : public PathStringConstructorTest {};
class InvalidPathStringAssignmentTest : public PathStringAssignmentTest{};

struct PathToStringTestParam {
	PathToStringTestParam(const std::string &path, const std::string &expectedResourceName,
		const std::string &expectedSubRootPath, const std::string &expectedSubRootName,
		const std::string &expectedPathWithoutFileName, const std::string &expectedToString,
		const std::string &expectedToNormalizedString) :
			path(path),
			expectedResourceName(expectedResourceName),
			expectedSubRootPath(expectedSubRootPath),
			expectedSubRootName(expectedSubRootName),
			expectedPathWithoutFileName(expectedPathWithoutFileName),
			expectedToString(expectedToString),
			expectedToNormalizedString(expectedToNormalizedString) {
	}

	std::string path; // input
	std::string expectedResourceName;
	std::string expectedSubRootPath;
	std::string expectedSubRootName;
	std::string expectedPathWithoutFileName;
	std::string expectedToString; // test Path::toString(false)
	std::string expectedToNormalizedString; // test Path::toString(true)
};

class PathToStringTest : public testing::TestWithParam<PathToStringTestParam> {
	protected:
		virtual void SetUp() {
			const PathToStringTestParam param = GetParam(); // test parameter

			this->path = Path(param.path, true);
			this->expectedResourceName = param.expectedResourceName;
			this->expectedSubRootPath = param.expectedSubRootPath;
			this->expectedSubRootName = param.expectedSubRootName;
			this->expectedPathWithoutFileName = param.expectedPathWithoutFileName;
			this->expectedToString = param.expectedToString;
			this->expectedToNormalizedString = param.expectedToNormalizedString;
		}

		virtual void TearDown() {}

	protected:
		Path path;
		std::string expectedResourceName;
		std::string expectedSubRootPath;
		std::string expectedSubRootName;
		std::string expectedPathWithoutFileName;
		std::string expectedToString;
		std::string expectedToNormalizedString;
};

struct IsParentOfTestParam {
	IsParentOfTestParam(const std::string &parent, const std::string &child, bool expectedIsParentOf)
		: parent(parent),
		  child(child),
		  expectedIsParentOf(expectedIsParentOf) {
	}

	std::string parent; // valid path input: to create the test subject Path(parent)
	std::string child; // valid path input: to test Path(parent).isParentOf(child)
	bool expectedIsParentOf;
};

class IsParentOfTest : public testing::TestWithParam<IsParentOfTestParam> {
	protected:
		virtual void SetUp() {
			const IsParentOfTestParam param = GetParam(); // test parameter

			this->parent = Path(param.parent, true);
			this->child = Path(param.child, true);
			this->expectedIsParentOf = param.expectedIsParentOf;
		}

		virtual void TearDown() {}

	protected:
		Path parent;
		Path child;
		bool expectedIsParentOf;
};

struct ReplaceParentPathParam {
		ReplaceParentPathParam(const Path &path, const Path &currentParent, const Path &newParent, const Path &expectedResult)
		: path(path),
		  currentParent(currentParent),
		  newParent(newParent),
		  expectedResult(expectedResult) {
	}

	Path path; // valid path input: to create the test subject
	Path currentParent;
	Path newParent;
	Path expectedResult; // the new path after apply Path::replaceParentPath, so: expectedResult = path.replaceParentPath(currentParent, newParent)
};

class ReplaceParentPathTest : public testing::TestWithParam<ReplaceParentPathParam> {
	protected:
		virtual void SetUp() {
			const ReplaceParentPathParam param = GetParam(); // test parameter

			this->path = param.path;
			this->currentParent = param.currentParent;
			this->newParent = param.newParent;
			this->expectedResult = param.expectedResult;
		}

		virtual void TearDown() {}

	protected:
		Path path;
		Path currentParent;
		Path newParent;
		Path expectedResult; // the new path after apply Path::replaceParentPath, so: expectedResult = path.replaceParentPath(currentParent, newParent)
};

struct IsRootTestParam {
	IsRootTestParam(const std::string &path, bool expectedResult)
		: path(path),
		  expectedResult(expectedResult) {
	}

	std::string path; // path input: to create the test subject
	bool expectedResult; // true if path is root ("/", "//", etc) false otherwise (includes invalid cases)
};

class IsRootTest : public testing::TestWithParam<IsRootTestParam> {
	protected:
		virtual void SetUp() {
			const IsRootTestParam param = GetParam(); // test parameter

			this->pathFromConstructor = Path(param.path);
			this->pathFromStringAssignment = param.path;
			this->expectedResult = param.expectedResult;
		}

		virtual void TearDown() {}

	protected:
		Path pathFromConstructor;
		Path pathFromStringAssignment;
		bool expectedResult; // the new path after apply Path::replaceParentPath, so: expectedResult = path.replaceParentPath(currentParent, newParent)
};

//BEGIN end classes

//BEGIN test data

const std::vector<std::string> validPaths = {
	"/",
	"/a/b/c/v/b/n/m/",
	"/a/b/c/v/b/n",
	"/root/blazing/data.1.parquet",
	"/root/blazing/data_1.simpatico",
	"/mnt/netdisck/blazing/data_1.simpatico",
	"/home/user34/dat.csv",
	"/home/user_45.g3/Desktop/fodlers/this spaced folder/",
	"/home/user_45.g3/Desktop/fodlers/this spaced folder/has_a_file.txt"
	"/home/user_45.g3///Desktop/fodlers///this spaced folder//has_a_file.txt",
	"/.", // equals to /
	"/home/user34/./Desktop/", // equals to /home/user34/Desktop/
};

const std::vector<std::string> invalidPaths = {
	"",
	"cat",
	"../../",
	".",
	"..",
	"/..",
	"/data/../../relative",
	"/home/user_45.g3/Desktop/fodlers/this spaced folder/../has_a_file.txt",
	"ult/home/percy/Desktop/cat_images/"
};

const std::vector<PathToStringTestParam> pathToStringTestEntries = {
	PathToStringTestParam(
		"/", // input path
		"", // expected resource name
		"", // expected sub root path
		"", // expected sub root name
		"", // expected path without resource name
		"/", // expected to string
		"/" // expected to string (normalize = true)
	),
	PathToStringTestParam(
		"//", // input path
		"", // expected resource name
		"", // expected sub root path
		"", // expected sub root name
		"", // expected path without resource name
		"//", // expected to string
		"/" // expected to string (normalize = true)
	),
	PathToStringTestParam(
		"/dir1/file1.txt", // input path
		"file1.txt", // expected resource name
		"/dir1/", // expected sub root path
		"dir1", // expected sub root name
		"/dir1/", // expected path without resource name
		"/dir1/file1.txt", // expected to string
		"/dir1/file1.txt" // expected to string (normalize = true)
	),
	PathToStringTestParam(
		"/dir-1/dir2/file1.txt", // input path
		"file1.txt", // expected resource name
		"/dir-1/", // expected sub root path
		"dir-1", // expected sub root name
		"/dir-1/dir2/", // expected path without resource name
		"/dir-1/dir2/file1.txt", // expected to string
		"/dir-1/dir2/file1.txt" // expected to string (normalize = true)
	),
	PathToStringTestParam(
		"/usr/share////wallpapers///Next/contents//images/////3200x1800.png", // input path
		"3200x1800.png", // expected resource name
		"/usr/", // expected sub root path
		"usr", // expected sub root name
		"/usr/share////wallpapers///Next/contents//images/////", // expected path without resource name
		"/usr/share////wallpapers///Next/contents//images/////3200x1800.png", // expected to string
		"/usr/share/wallpapers/Next/contents/images/3200x1800.png" // expected to string (normalize = true)
	),
	PathToStringTestParam(
		"/usr.disk1/share/wallpapers/", // input path
		"wallpapers", // expected resource name
		"/usr.disk1/", // expected sub root path
		"usr.disk1", // expected sub root name
		"/usr.disk1/share/", // expected path without resource name
		"/usr.disk1/share/wallpapers/", // expected to string
		"/usr.disk1/share/wallpapers/" // expected to string (normalize = true)
	),
	PathToStringTestParam(
		"/opt///share/tpch-1gb//", // input path
		"tpch-1gb", // expected resource name
		"/opt/", // expected sub root path
		"opt", // expected sub root name
		"/opt///share/", // expected path without resource name
		"/opt///share/tpch-1gb//", // expected to string
		"/opt/share/tpch-1gb/" // expected to string (normalize = true)
	),
	PathToStringTestParam(
		"//a////b//c/de///f////k////////g.dat", // input path
		"g.dat", // expected resource name
		"/a/", // expected sub root path
		"a", // expected sub root name
		"//a////b//c/de///f////k////////", // expected path without resource name
		"//a////b//c/de///f////k////////g.dat", // expected to string
		"/a/b/c/de/f/k/g.dat" // expected to string (normalize = true)
	)
};

const std::vector<IsParentOfTestParam> isParentOfTestEntries = {
	IsParentOfTestParam(
		"/data/", // input parent
		"/data/img.png", // input child
		true // expected isParentOf: true if input parent contains input child, false otherwise
	),
	IsParentOfTestParam(
		"//data/", // input parent
		"///data//img.png", // input child
		true // expected isParentOf: true if input parent contains input child, false otherwise
	),
	IsParentOfTestParam(
		"/data//", // input parent
		"//data/img.png", // input child
		true // expected isParentOf: true if input parent contains input child, false otherwise
	),
	IsParentOfTestParam(
		"//data///", // input parent
		"/data//img.png", // input child
		true // expected isParentOf: true if input parent contains input child, false otherwise
	),
	IsParentOfTestParam(
		"////data//", // input parent
		"//data///img.png", // input child
		true // expected isParentOf: true if input parent contains input child, false otherwise
	),
	IsParentOfTestParam(
		"///usr//share/disk1/data.txt", // input parent
		"/data//img.png", // input child
		false // expected isParentOf: true if input parent contains input child, false otherwise
	),
	IsParentOfTestParam(
		"///usr//share/disk1/data.txt", // input parent
		"///usr//share/disk1/data.txt/", // input child
		true // expected isParentOf: true if input parent contains input child, false otherwise
	),
	IsParentOfTestParam(
		"///usr//share/disk1/data.txt/", // input parent
		"///usr//share/disk1/data.txt", // input child
		false // expected isParentOf: true if input parent contains input child, false otherwise
	),
	IsParentOfTestParam(
		"///usr//share/disk1/data.txt/", // input parent
		"///usr//share/disk1/data.txt/", // input child
		true // expected isParentOf: true if input parent contains input child, false otherwise
	),
	IsParentOfTestParam(
		"invalid", // input parent
		"not valid", // input child
		false // expected isParentOf: true if input parent contains input child, false otherwise
	),
	IsParentOfTestParam(
		"invalid/", // input parent
		"not valid/", // input child
		false // expected isParentOf: true if input parent contains input child, false otherwise
	)
};

const std::vector<ReplaceParentPathParam> replaceParentPathTestEntries = {
	ReplaceParentPathParam(
		Path("/data/img.png", true), // input path
		Path("/data/", true), // currentParent
		Path("/usr/share/", true), // newParent
		Path("/usr/share/img.png", true) // expectedResult
	),
	ReplaceParentPathParam(
		Path("//data///img.png", true), // input path
		Path("/data////", true), // currentParent
		Path("/usr/share//", true), // newParent
		Path("/usr/share//img.png", true) // expectedResult
	)
};

const std::vector<IsRootTestParam> isRootTestEntries = {
	IsRootTestParam(
		"/data/", // input
		false // expected result: true if path is root ("/", "//", etc) false otherwise (includes invalid cases)
	),
	IsRootTestParam(
		"dasd asdad", // input
		false // expected result: true if path is root ("/", "//", etc) false otherwise (includes invalid cases)
	),
	IsRootTestParam(
		"//", // input
		true // expected result: true if path is root ("/", "//", etc) false otherwise (includes invalid cases)
	),
	IsRootTestParam(
		"/", // input
		true // expected result: true if path is root ("/", "//", etc) false otherwise (includes invalid cases)
	),
	IsRootTestParam(
		"//////", // input
		true // expected result: true if path is root ("/", "//", etc) false otherwise (includes invalid cases)
	)
};

//END test data

static void checkValidTest(const Path &path, const std::string &param) {
	const Path other(param);

	EXPECT_TRUE(path.isValid());
	EXPECT_FALSE(path.isEmpty());
	EXPECT_EQ(path.toString(false), param);
	EXPECT_EQ(path, other);
}

static void checkInvalidTest(const Path &path) {
	EXPECT_FALSE(path.isValid());
}

TEST_P(ValidPathStringConstructorTest, CheckValidPathsUsingStringConstructor) {
	checkValidTest(path, GetParam());
}

TEST_P(ValidPathStringAssignmentTest, CheckValidPathsUsingStringAssignment) {
	checkValidTest(path, GetParam());
}

TEST_P(InvalidPathStringConstructorTest, CheckInvalidPathsUsingStringConstructor) {
	checkInvalidTest(path);
}

TEST_P(InvalidPathStringAssignmentTest, CheckInvalidPathsUsingStringAssignment) {
	checkInvalidTest(path);
}

TEST_P(PathToStringTest, CheckPathToStringMethods) {
	EXPECT_TRUE(path.isValid());
	EXPECT_EQ(path.getResourceName(), expectedResourceName);
	EXPECT_EQ(path.getSubRootPath(), Path(expectedSubRootPath, true));
	EXPECT_EQ(path.getSubRootPath().getResourceName(), expectedSubRootName);
	EXPECT_EQ(path.getParentPath(), Path(expectedPathWithoutFileName, true));
	EXPECT_EQ(path.toString(false), expectedToString);
	EXPECT_EQ(path.toString(true), expectedToNormalizedString);
}

TEST_P(IsParentOfTest, CheckIsParentOf) {
	const bool result = parent.isParentOf(child);

	EXPECT_EQ(result, expectedIsParentOf);
}

TEST_P(ReplaceParentPathTest, CheckReplaceParentPath) {
	const Path result = path.replaceParentPath(currentParent, newParent);

	EXPECT_EQ(result, expectedResult);
}

TEST_P(IsRootTest, CheckIsRoot) {
	const bool resultFromConstructor = pathFromConstructor.isRoot();

	EXPECT_EQ(resultFromConstructor, expectedResult);

	const bool resultFromStringAssignment = pathFromStringAssignment.isRoot();

	EXPECT_EQ(resultFromStringAssignment, expectedResult);
}

INSTANTIATE_TEST_CASE_P(ValidPathsForStringConstructorTestCase, ValidPathStringConstructorTest, testing::ValuesIn(validPaths));
INSTANTIATE_TEST_CASE_P(ValidPathsForStringAssignmentTestCase, ValidPathStringAssignmentTest, testing::ValuesIn(validPaths));

INSTANTIATE_TEST_CASE_P(InvalidPathsForStringConstructorTestCase, InvalidPathStringConstructorTest, testing::ValuesIn(invalidPaths));
INSTANTIATE_TEST_CASE_P(InvalidPathsStringAssignmentTestCase, InvalidPathStringAssignmentTest, testing::ValuesIn(invalidPaths));

INSTANTIATE_TEST_CASE_P(PathToStringTestCase, PathToStringTest, testing::ValuesIn(pathToStringTestEntries));

INSTANTIATE_TEST_CASE_P(IsParentOfTestCase, IsParentOfTest, testing::ValuesIn(isParentOfTestEntries));

INSTANTIATE_TEST_CASE_P(ReplaceParentPathTestCase, ReplaceParentPathTest, testing::ValuesIn(replaceParentPathTestEntries));

INSTANTIATE_TEST_CASE_P(IsRootTestCase, IsRootTest, testing::ValuesIn(isRootTestEntries));
