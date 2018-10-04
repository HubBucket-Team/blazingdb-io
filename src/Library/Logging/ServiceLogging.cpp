#include "Library/Logging/ServiceLogging.h"
#include "Library/Logging/GenericOutput.h"

namespace Library {
    namespace Logging {
        ServiceLogging::ServiceLogging() {
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
