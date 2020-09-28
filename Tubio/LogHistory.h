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
        static void Init();
        static void PostExit();
        static void Update();

        static void LoadSaveFileCache();
        static void Save();

        static std::vector<LogEntry*>* GetSessionLogHistory() { return history; }

        /// <summary>
        /// Will return all logs ever in json format. You can specify a max_age and a max_num
        /// </summary>
        /// <param name="max_age">Max age to return</param>
        /// <param name="max_num">Maximum amount to return</param>
        /// <returns></returns>
        static JasonPP::JsonArray GetCompleteLogHistoryAsJson(time_t max_age, std::size_t max_num);

        /// <summary>
        /// Will clear the log history
        /// </summary>
        static bool ClearLogHistory();

    private:
        static void AddLogToHistory(LogEntry* newEntry);

        static std::vector<LogEntry*>* history;
        static JasonPP::JsonArray saveFileCache;
        static time_t lastSave;
        static bool didHistoryChangeSinceLastSave;

        friend Logger;
    };
}
