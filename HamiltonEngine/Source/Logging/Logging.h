#pragma once

#define HAMILTON_LOG(Category, Level, ...) {\
    SPDLOG_LOGGER_CALL(HamiltonEngine::Logging::GetLogger(HamiltonEngine::Logging::LogCategory::Category), \
        static_cast<spdlog::level::level_enum>(HamiltonEngine::Logging::LogLevel::Level),  __VA_ARGS__); \
}

namespace HamiltonEngine::Logging
{
    enum class LogCategory : uint32_t
    {
        All, //Do not use this directly, it's exposed here so we avoid a bunch of potential off by one errors 
        General,
        Physics,
        Graphics,
        Count
    };

    enum class LogLevel : uint32_t 
    {
        Trace = SPDLOG_LEVEL_TRACE,
        Debug = SPDLOG_LEVEL_DEBUG,
        Info = SPDLOG_LEVEL_INFO,
        Warning = SPDLOG_LEVEL_WARN,
        Error = SPDLOG_LEVEL_ERROR,
        Critical = SPDLOG_LEVEL_CRITICAL,
        Off = SPDLOG_LEVEL_OFF,
        dssadsa
    };

    spdlog::logger* GetLogger(LogCategory Category);
}