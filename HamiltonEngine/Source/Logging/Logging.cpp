#include "PrecompiledHeader/Pch.h"

#include "Logging.h"
#include "Configuration/ConfigurationVariable.h"

#define FMT_UNICODE 0
#include <spdlog/spdlog.h>
#include <spdlog/sinks/callback_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace 
{
    HamiltonEngine::ConfigurationVariable<std::string> LogDir("LogDir", "Logs");
    HamiltonEngine::ConfigurationVariable<bool> LogToConsole("LogToConsole", true);

    struct SpdLogInfo
    {
        SpdLogInfo() = delete;
        SpdLogInfo(SpdLogInfo&) = delete;
        
        SpdLogInfo(const char* Name, spdlog::level::level_enum LogLevel)
        {
            // Create log for this category
            std::filesystem::path FilePath{ LogDir.Get()};
            FilePath.append(Name);
            FilePath.replace_extension(".log");
            const std::string CategoryFileName = FilePath.string();

            auto CategoryFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(CategoryFileName);
            CategoryFileSink->set_level(LogLevel);

            // Create log sink for the All log that concatinates the category logs
            FilePath.replace_filename("All.log");
            const std::string AllFileName = FilePath.string();

            auto AllFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(AllFileName);
            AllFileSink->set_level(LogLevel);
            
            if (LogToConsole) 
            {
                // Create console sink so we see warnings in std out
                auto ConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
                Logger = new(&Storage) spdlog::logger(Name, { CategoryFileSink, ConsoleSink, AllFileSink });    
            }
            else 
            {
                Logger = new(&Storage) spdlog::logger(Name, { CategoryFileSink, AllFileSink });    
            }
        }

        ~SpdLogInfo() 
        {
            Logger->~logger();
            memset(&Storage, 0, sizeof(Storage));
        }
        
        using StorageType = std::aligned_storage_t<sizeof(spdlog::logger), alignof(spdlog::logger)>;
        StorageType Storage;
        spdlog::logger* Logger;
    };

    constexpr auto LogCount =  static_cast<std::underlying_type_t<HamiltonEngine::Logging::LogCategory>>(HamiltonEngine::Logging::LogCategory::Count);
    SpdLogInfo Logs[LogCount] = { 
        //SpdLogInfo("All", spdlog::level::info, nullptr),
        SpdLogInfo("General", spdlog::level::info),
        SpdLogInfo("Physics", spdlog::level::info),
        SpdLogInfo("Graphics",spdlog::level::info),
        SpdLogInfo("Configuration",spdlog::level::info),
        SpdLogInfo("Serialization",spdlog::level::info)
    };
}

namespace HamiltonEngine::Logging 
{
    void LogMessageVairadic(LogCategory Category, 
        LogLevel LogLevel,
        const char* Message,
        ...)
    {
        constexpr int MAX_LEN = 1024;
        char Buff[MAX_LEN];
       
        va_list Args;
        va_start(Args, Message);
        vsnprintf(Buff, sizeof(Buff), Message, Args);
        va_end(Args);

        using PodType = std::underlying_type_t<LogCategory>;
        spdlog::logger* Logger = Logs[static_cast<PodType>(Category)].Logger;

        SPDLOG_LOGGER_CALL(Logger, static_cast<spdlog::level::level_enum>(LogLevel), Buff);
    
    }
}
