#pragma once

#include <spdlog/spdlog.h>

#include <memory>

namespace neith {

class Log {
public:
    static void Init();
    inline static std::shared_ptr<spdlog::logger> GetInternalLogger() { return mInternalLogger; }
    inline static std::shared_ptr<spdlog::logger> GetAppLogger() { return mAppLogger; }

private:
    static std::shared_ptr<spdlog::logger> mInternalLogger;
    static std::shared_ptr<spdlog::logger> mAppLogger;
};
}  // namespace neith

#define NT_INTER_CRITICAL(...) ::neith::Log::GetInternalLogger()->critical(__VA_ARGS__)
#define NT_INTER_ERROR(...) ::neith::Log::GetInternalLogger()->error(__VA_ARGS__)
#define NT_INTER_WARN(...) ::neith::Log::GetInternalLogger()->warn(__VA_ARGS__)
#define NT_INTER_INFO(...) ::neith::Log::GetInternalLogger()->info(__VA_ARGS__)
#define NT_INTER_TRACE(...) ::neith::Log::GetInternalLogger()->trace(__VA_ARGS__)

#define NT_CRITICAL(...) ::neith::Log::GetAppLogger()->critical(__VA_ARGS__)
#define NT_ERROR(...) ::neith::Log::GetAppLogger()->error(__VA_ARGS__)
#define NT_WARN(...) ::neith::Log::GetAppLogger()->warn(__VA_ARGS__)
#define NT_INFO(...) ::neith::Log::GetAppLogger()->info(__VA_ARGS__)
#define NT_TRACE(...) ::neith::Log::GetAppLogger()->trace(__VA_ARGS__)
