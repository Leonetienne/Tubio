#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <fstream>
#include "LogTypes.h"
#include "XGConfig.h"
#include "JasonPP.hpp"

namespace Logging
{
    class Logger;

    class LogEntry
    {
    public:
        std::string compiledMessage;
        std::string message;
        std::string identifier;
        LOG_TYPE type;
        std::time_t timestamp;

        JasonPP::JsonBlock GetAsJson();
    };

    class LogHistory
    {
    public:
        static void PreInit();
        static void PostExit();

        static void Save();

        static std::vector<LogEntry*>* GetLogHistory() { return history; }

    private:
        static void AddLogToHistory(LogEntry* newEntry);

        static std::vector<LogEntry*>* history;

        friend Logger;
    };
}
