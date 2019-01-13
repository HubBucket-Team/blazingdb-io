#include "Library/Logging/ServiceLogging.h"
#include "Library/Logging/GenericOutput.h"
#include "Library/Logging/FileOutput.h"
#include <iostream>

namespace Library {
    namespace Logging {
        ServiceLogging::ServiceLogging() {
    		//allows unit tests to run dumping to some log
		auto output = new FileOutput("/tmp/blazing-logger.log", true);
		std::cout<<"insanity test"<<std::endl;
    		setLogOutput(output);
        }

        ServiceLogging::~ServiceLogging() {
            if (output) {
                delete output;
                output = nullptr;
            }
        }

        void ServiceLogging::setLogData(std::string&& data) {
            output->flush(data);
        }

        void ServiceLogging::setLogData(const std::string& data) {
            output->flush(data);
        }

        void ServiceLogging::setLogOutput(GenericOutput* value) {
            if (output) {
                delete output;
            }
            output = value;
        }

        void ServiceLogging::setNodeIdentifier(const int nodeInd) {
        	output->setNodeIdentifier(nodeInd);
        }
    }
}
