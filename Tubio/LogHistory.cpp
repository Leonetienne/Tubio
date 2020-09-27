#include "LogHistory.h"

using namespace Logging;

void LogHistory::PreInit()
{
    history = new std::vector<LogEntry*>();
    lastSave = time(0);
    didHistoryChangeSinceLastSave = false;

    return;
}

void LogHistory::PostExit()
{
    Save();


    for (std::size_t i = 0; i < history->size(); i++)
    {
        delete history->at(i);
        history->at(i) = nullptr;
    }

    delete history;
    history = nullptr;

    return;
}

void LogHistory::Update()
{
    if ((time(0) - lastSave > XGConfig::logging.autosave_interval) && (didHistoryChangeSinceLastSave))
    {
        // Mutex gets reset in Save();
        Save();
    }

    return;
}

void LogHistory::Save()
{
    std::stringstream textfile;
    JasonPP::Json newJsonLogs = JasonPP::JsonArray();

    for (std::size_t i = 0; i < history->size(); i++)
    {
        textfile << history->at(i)->compiledMessage << std::endl;
        newJsonLogs.AsArray += history->at(i)->GetAsJson();
    }

    std::ofstream ofs;
    ofs.open(XGConfig::logging.logfile_text, std::ios::app);
    ofs << textfile.str();
    ofs.close();

    // You can't just append to a json array file...
    // You have to first parse it, append to it, and then rewrite the complete file
    {
        JasonPP::Json allLogs;

        if (FileSystem::Exists(XGConfig::logging.logfile_json))
        {
            std::string fileContent = FileSystem::ReadFile(XGConfig::logging.logfile_json);
            if (JasonPP::IsJsonValid(fileContent))
            {
                allLogs.Parse(fileContent);
                if (allLogs.GetDataType() != JasonPP::JDType::ARRAY)
                {
                    // Json file is fucked (wrong format). Reset to empty array
                    allLogs.SetArrayData(JasonPP::JsonArray());
                }
            }
            else
            {
                // Json file is fucked (wrong syntax). Reset to empty array
                allLogs.SetArrayData(JasonPP::JsonArray());
            }
        }
        else
        {
            // Json file is fucked (doesn't exist). Reset to empty array
            allLogs.SetArrayData(JasonPP::JsonArray());
        }

        allLogs.AsArray.Merge(newJsonLogs.AsArray);
        ofs.open(XGConfig::logging.logfile_json);
        ofs << allLogs.Render();
        ofs.close();

    }

    lastSave = time(0);
    didHistoryChangeSinceLastSave = false;

    return;
}

JasonPP::JsonArray LogHistory::GetCompleteLogHistoryAsJson()
{
    Save();

    // Logfile does not exist, just return an empty array
    if (!FileSystem::Exists(XGConfig::logging.logfile_json))
    {
        std::cout << "no log file" << std::endl;
        return JasonPP::JsonArray();
    }
    else
    {
        // Logfile exists
        std::string file_contents = FileSystem::ReadFile(XGConfig::logging.logfile_json);
        if (JasonPP::IsJsonValid(file_contents))
        {
            JasonPP::Json logs;
            logs.Parse(file_contents);

            if (logs.GetDataType() == JasonPP::JDType::ARRAY)
            {
                logs.AsArray.Sort("timestamp", JasonPP::JSON_ARRAY_SORT_MODE::NUM_DESC);
                return logs.AsArray;
            }
            else
            {
                std::cout << "invalid format log file" << std::endl;
                ClearLogHistory(); // The json logfile is fucked
                return JasonPP::JsonArray();
            }
        }
        else
        {
            std::cout << "invalid syntax log file" << std::endl;
            std::cout << file_contents << std::endl;
            ClearLogHistory(); // The json logfile is fucked
            return JasonPP::JsonArray();
        }
    }
}

bool LogHistory::ClearLogHistory()
{
    FileSystem::Delete(XGConfig::logging.logfile_json);
    FileSystem::Delete(XGConfig::logging.logfile_text);

    Save();

    return true;
}

void LogHistory::AddLogToHistory(LogEntry* newEntry)
{
    history->push_back(newEntry);
    didHistoryChangeSinceLastSave = true;

    return;
}

std::vector<LogEntry*>* LogHistory::history;
time_t LogHistory::lastSave;
bool LogHistory::didHistoryChangeSinceLastSave;

JasonPP::JsonBlock LogEntry::GetAsJson()
{
    return JasonPP::JsonBlock({
        Ele("compiledMessage", compiledMessage),
        Ele("message", message),
        Ele("identifier", identifier),
        Ele("additional_information", additional_information),
        Ele("type", (int)type),
        Ele("timestamp", (long long int)timestamp),
    });
}
