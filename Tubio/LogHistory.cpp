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
    JasonPP::Json jsonFile = JasonPP::JsonArray();

    for (std::size_t i = 0; i < history->size(); i++)
    {
        textfile << history->at(i)->compiledMessage << std::endl;
        jsonFile.AsArray += history->at(i)->GetAsJson();
    }

    std::ofstream ofs;
    ofs.open(XGConfig::logging.logfile_text, std::ios::app);
    ofs << textfile.str();
    ofs.close();

    ofs.open(XGConfig::logging.logfile_json, std::ios::app);
    ofs << jsonFile.Render();
    ofs.close();

    lastSave = time(0);
    didHistoryChangeSinceLastSave = false;

    return;
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
        Ele("compiledMessage", message),
        Ele("message", compiledMessage),
        Ele("identifier", identifier),
        Ele("type", (int)type),
        Ele("timestamp", (long long int)timestamp),
    });
}
