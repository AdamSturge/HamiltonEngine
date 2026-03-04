#include "PrecompiledHeader/Pch.h"

#include "Logging.h"
#include "Configuration/ConfigurationVariable.h"

namespace 
{
    HamiltonEngine::ConfigurationVariable<std::string> LogDir("LogDir", "Logs");

    /*TODO:
        - Figure out how to add name of child log to parent log
        - Try using unique ptrs instead of shared (or even non-heap memory)
    */
    struct LogInfo
    {
        LogInfo() = delete;
        LogInfo(LogInfo&) = delete;
        
        LogInfo(const char* Name,
            spdlog::level::level_enum LogLevel,
            LogInfo* ParentLog)
        {
            std::filesystem::path FilePath{ LogDir.Get()};
            FilePath.append(Name);
            FilePath.replace_extension(".log");
            const std::string FileName = FilePath.string();

            auto FileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(FileName);
            FileSink->set_level(LogLevel);
            
            if (ParentLog) 
            {
                auto ParentLogCallback = std::make_shared<spdlog::sinks::callback_sink_mt>(
                    [ParentLog, Name](const spdlog::details::log_msg& msg)
                    {
                        ParentLog->Logger->log(msg.time, msg.source, msg.level, msg.payload);
                    });
                ParentLogCallback->set_level(LogLevel);
                
                Logger = std::make_shared<spdlog::logger>(spdlog::logger(Name, { FileSink, ParentLogCallback }));
            }
            else 
            {
                Logger = std::make_shared<spdlog::logger>(spdlog::logger(Name, { FileSink }));
            }
        }
        
        std::shared_ptr<spdlog::logger> Logger;
    };

    constexpr auto LogCount =  static_cast<std::underlying_type_t<HamiltonEngine::Logging::LogCategory>>(HamiltonEngine::Logging::LogCategory::Count);
    LogInfo Logs[LogCount] = { 
        LogInfo("All", spdlog::level::info, nullptr),
        LogInfo("General", spdlog::level::info, &Logs[0]),
        LogInfo("Physics", spdlog::level::info, &Logs[0]),
        LogInfo("Graphics",spdlog::level::info, &Logs[0])
    };
}

namespace HamiltonEngine::Logging 
{
    spdlog::logger* GetLogger(LogCategory Category)
    {
        using PodType = std::underlying_type_t<LogCategory>;
        return Logs[static_cast<PodType>(Category)].Logger.get();
    }
}
