#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <fstream>
#include "LogTypes.h"
#include "XGConfig.h"
#include "external_dependencies/leonetienne/JasonPP/JasonPP.hpp"

namespace Logging
{
    class Logger;

    class LogEntry
    {
    public:
        std::string compiledMessage;
        std::string message;
        std::string identifier;
        std::string additional_information;
        LOG_TYPE type;
        std::time_t timestamp;

        JasonPP::JsonBlock GetAsJson();
    };

    class LogHistory
    {
    public:
        static void PreInit();
        static void PostExit();
        static void Update();

        static void Save();

        static std::vector<LogEntry*>* GetSessionLogHistory() { return history; }
        static JasonPP::JsonArray GetCompleteLogHistoryAsJson();

        /// <summary>
        /// Will clear the log history
        /// </summary>
        static bool ClearLogHistory();

    private:
        static void AddLogToHistory(LogEntry* newEntry);

        static std::vector<LogEntry*>* history;
        static time_t lastSave;
        static bool didHistoryChangeSinceLastSave;

        friend Logger;
    };
}
