#include "PrecompiledHeader/Pch.h"

#include "Logging.h"


namespace HamiltonEngine::Logging 
{
    void ConfigureGlobalLoggingOptions(const char* DefaultLogFileName)
    {
        std::filesystem::path DefaultLogFilePath{ DefaultLogFileName };
        
        std::shared_ptr<spdlog::logger> DefaultLogger = spdlog::basic_logger_mt("DefaultLog", DefaultLogFilePath.string());

        spdlog::default_logger()->set_level(spdlog::level::trace);

        DefaultLogger->warn("Test Warning");
    }
}
