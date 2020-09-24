#include "LogHistory.h"

using namespace Logging;

void LogHistory::PreInit()
{
    history = new std::vector<LogEntry*>();

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

    return;
}

void LogHistory::AddLogToHistory(LogEntry* newEntry)
{
    history->push_back(newEntry);

    return;
}

std::vector<LogEntry*>* LogHistory::history;

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
