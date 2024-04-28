#include "simple_log_processor.h"

namespace logger {

    void SimpleLogProcessor::Process(LogRecord &log) {
        this->exporter->Export(log);
    }
}