#pragma once

//#define HAMILTON_LOG(Category, Level, ...) {\
//    SPDLOG_LOGGER_CALL(HamiltonEngine::Logging::GetLogger(HamiltonEngine::Logging::LogCategory::Category), \
//        static_cast<spdlog::level::level_enum>(HamiltonEngine::Logging::LogLevel::Level),  __VA_ARGS__); \
//}

#define HAMILTON_LOG(Category, Level, Message, ...) {\
    using namespace HamiltonEngine::Logging;\
    LogMessageVairadic(LogCategory::Category, LogLevel::Level, Message, __VA_ARGS__); \
}

namespace HamiltonEngine::Logging
{
    enum class LogCategory : uint32_t
    { 
        General,
        Physics,
        Graphics,
        Configuration,
        Serialization,
        Count
    };

    //Note that the order here matters as it must match spdlog
    enum class LogLevel : uint32_t 
    {
        Trace,
        Debug,
        Info,
        Warning,
        Error,
        Critical,
        Off ,
        Count
    };

    void LogMessageVairadic(LogCategory Category,
        LogLevel LogLevel,
        const char* Message, 
        ...);
}