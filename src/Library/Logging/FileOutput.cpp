#include "Library/Logging/FileOutput.h"
#include "Library/Logging/Logger.h"
#include <iostream>

namespace Library {
    namespace Logging {
        FileOutput::FileOutput(const std::string& filename, bool truncate) {
            auto mode = std::ios::out | (truncate ? std::ios::trunc : std::ios::app); 
            file.open(filename, mode);
        }

        FileOutput::~FileOutput() {
        }

        void FileOutput::flush(std::string&& log) {
            std::unique_lock<std::mutex> lock(mutex);
            file << log << std::endl;
        }

        void FileOutput::flush(const std::string& log) {
            std::unique_lock<std::mutex> lock(mutex);
            file << log << std::endl;
        }

        void FileOutput::setNodeIdentifier(const unsigned int nodeInd){
        	Logging::Logger().logInfo("Node index is " + std::to_string(nodeInd));
        }
    }
}
