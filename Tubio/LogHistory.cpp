#include "LogHistory.h"

using namespace Logging;

void LogHistory::PreInit()
{
    history = new std::vector<LogEntry*>();

    return;
}

void LogHistory::PostExit()
{
    for (unsigned int i = 0; i < history->size(); i++)
    {
        delete history->at(i);
        history->at(i) = nullptr;
    }

    delete history;
    history = nullptr;

    return;
}

void LogHistory::AddLogToHistory(LogEntry* _newEntry)
{
    history->push_back(_newEntry);

    return;
}

std::vector<LogEntry*>* LogHistory::history;
