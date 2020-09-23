#pragma once
#include <string>
#include <vector>
#include <ctime>
#include "LogTypes.h"

namespace Logging
{
    class Logger;

    class LogEntry
    {
    public:
        std::string message;
        std::string identifier;
        LOG_TYPE type;
        std::time_t timestamp;
    };

    class LogHistory
    {
    public:
        static void PreInit();
        static void PostExit();

        static std::vector<LogEntry*>* GetLogHistory() { return history; }

    private:
        static void AddLogToHistory(LogEntry* _newEntry);

        static std::vector<LogEntry*>* history;

        friend Logger;
    };
}
