#include "Logger.h"
#include "LogHistory.h"

using namespace Logging;

Logger::Logger(std::string identifier)
{
    isInitialized = true;

    std::stringstream ss;

    this->identifier = identifier;

    return;
}

void Logger::Clear()
{
    if (!IsInitializedSanityCheck()) return;

    cout.str("");
    type = LOG_TYPE::LOG;

    return;
}

void Logger::Set(std::string str)
{
    if (!IsInitializedSanityCheck()) return;

    Clear();
    cout << str;

    return;
}

std::string Logger::Flush()
{
    if (!IsInitializedSanityCheck()) return "";


    time_t currTime = time(0);
    char timeBuf[256];
#ifdef _WIN
    tm currTm;
    localtime_s(&currTm, &currTime);
    strftime(timeBuf, 100, "%d.%m.%Y - %T", &currTm);
#else
    tm* currTm = localtime(&currTime);
    strftime(timeBuf, 100, "%d.%m.%Y - %T", currTm);
#endif

    std::stringstream bufOut;
    bufOut << "<" << timeBuf << "> [" << identifier << "]" << TypeToPrefix(type) << ((additional_information.length() > 0) ? " " : "") << additional_information << ": " << cout.str();

    LogEntry* newEntry = new LogEntry;
    newEntry->message = cout.str();
    newEntry->compiledMessage = bufOut.str();
    newEntry->identifier = identifier;
    newEntry->additional_information = additional_information;
    newEntry->timestamp = currTime;
    newEntry->type = type;
    LogHistory::AddLogToHistory(newEntry);

    std::cout << bufOut.str() << std::endl;

    Clear();

    return "";
}

std::string Logger::Type(LOG_TYPE type)
{
    if (!IsInitializedSanityCheck()) return "";

    this->type = type;
    return "";
}

std::string Logger::TypeToPrefix(LOG_TYPE type)
{
    switch (type)
    {
    case LOG_TYPE::LOG:
        return "";

    case LOG_TYPE::WARN:
        return " [WARN]";

    case LOG_TYPE::ERR:
        return " [ERROR]";

    default:
        return "";
    }

    return "";
}

std::string Logger::TypeToColor(LOG_TYPE _type)
{
    /*switch (_type)
    {
    case LOG_TYPE::LOG:
        return "\033[0m";

    case LOG_TYPE::WARN:
        return "\033[33m";

    case LOG_TYPE::ERR:
        return "\033[31m";

    default:
        return "\033[0m";
    }

    return "\033[0m";*/
    return "";
}

bool Logger::IsInitializedSanityCheck()
{
    if (!isInitialized)
    {
        Logger log("Logger"); //A Log object cannot always have a Log-member because of its recursive nature. Only create them when needed!
        log.cout << log.Err() << "Attempted to use logger object without being initialized!";
        log.Flush();
        return false;
    }

    return true;
}