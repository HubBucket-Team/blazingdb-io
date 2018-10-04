/*
 * FileSystemType.h
 *
 *  Created on: Nov 24, 2017
 */

#ifndef _BZ_FILESYSTEM_TYPE_H_
#define _BZ_FILESYSTEM_TYPE_H_

#include <string>

enum class FileSystemType : char {
	UNDEFINED,
	LOCAL,
	HDFS,
	S3,
	NFS4
};

enum class FileType : char {
	UNDEFINED,
	FILE,
	DIRECTORY,
};

//returns string representation of FileSystemType
const std::string fileSystemTypeName(FileSystemType fileSystemType);

//returns string representation of FileType
const std::string fileTypeName(FileType fileType);

#endif /* _BZ_FILESYSTEM_TYPE_H_ */
