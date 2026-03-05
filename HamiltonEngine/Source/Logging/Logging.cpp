#include "PrecompiledHeader/Pch.h"

#include "Logging.h"
#include "Configuration/ConfigurationVariable.h"

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
        SpdLogInfo("Graphics",spdlog::level::info)
    };
}

namespace HamiltonEngine::Logging 
{
    spdlog::logger* GetLogger(LogCategory Category)
    {
        using PodType = std::underlying_type_t<LogCategory>;
        return Logs[static_cast<PodType>(Category)].Logger;
    }
}
