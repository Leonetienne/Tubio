#include "LogHistory.h"

using namespace Logging;

void LogHistory::PreInit()
{
    history = new std::vector<LogEntry*>();
    lastSave = time(0); // now
    didHistoryChangeSinceLastSave = false;
    LoadSaveFileCache();

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
        LoadSaveFileCache();
        JasonPP::Json savefile;
        savefile.SetArrayData(saveFileCache);
        savefile.AsArray.Merge(newJsonLogs.AsArray);
        ofs.open(XGConfig::logging.logfile_json);
        ofs << savefile.Render();
        ofs.close();

    }

    lastSave = time(0);
    didHistoryChangeSinceLastSave = false;

    return;
}

void LogHistory::LoadSaveFileCache()
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
    saveFileCache.Clear();
    saveFileCache.CloneFrom(allLogs.AsArray);
    return;
}

JasonPP::JsonArray LogHistory::GetCompleteLogHistoryAsJson(time_t max_age, std::size_t max_num)
{
    JasonPP::JsonArray arr
        ;
    
    for (std::size_t i = 0; i < history->size(); i++)
    {
        arr += history->at(i)->GetAsJson();
    }
    arr.Merge(saveFileCache);
    arr.Sort("timestamp", JasonPP::JSON_ARRAY_SORT_MODE::NUM_DESC);

    if ((max_age == -1) && (max_num == (std::size_t)-1)) return arr;

    JasonPP::JsonArray cutArr;
    for (std::size_t i = 0; ((i < arr.Size()) && (i < max_num)); i++)
    {
        // If max_age is > 0, we have to check against the max age
        if (max_age > 0)
        {
            if (arr[i].AsJson.DoesExist("timestamp"))
            {
                if (arr[i]["timestamp"].GetDataType() == JasonPP::JDType::INT)
                {
                    if ((time(0) - arr[i]["timestamp"].AsInt) < max_age)
                    {
                        cutArr += arr[i];
                    }
                }
            }
        }
        else // If not, just insert it
        {
            cutArr += arr[i];
        }
    }
    return cutArr;
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
JasonPP::JsonArray LogHistory::saveFileCache;
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
