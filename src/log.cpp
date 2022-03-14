#include "log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/stdout_sinks.h>
namespace neith {

    std::shared_ptr<spdlog::logger> Log::mInternalLogger;
    std::shared_ptr<spdlog::logger> Log::mAppLogger;

    void Log::Init() {
        spdlog::set_pattern("[%T] [%n] [%^%l%$] %v");
        mInternalLogger = spdlog::stdout_color_mt("NEITH");
        mInternalLogger->set_level(spdlog::level::trace);

        mAppLogger = spdlog::stdout_color_mt("APP");
        mAppLogger->set_level(spdlog::level::trace);
    }
}
