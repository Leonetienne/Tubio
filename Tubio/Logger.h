#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include "LogTypes.h"

namespace Logging
{
    class Logger
    {
    public:

    public:
        //Creates a Logger object. Pass an identifier such as MySQL
        Logger(std::string identifier);

        //Clears the buffered string and resets the log type to default
        void Clear();

        //Sets the buffered string
        void Set(std::string str);

        //Sets additional information to be appended after the identifier
        void SetAdditionalInformation(std::string str) { additional_information = str; };

        //Prints the buffered string to the console and clears it
        std::string Flush();

        //Sets a custom log type
        std::string Type(LOG_TYPE type);

        //Sets the log type to warning
        std::string Warn() { return Type(LOG_TYPE::WARN); }

        //Sets the log type to error
        std::string Err() { return Type(LOG_TYPE::ERR); }

        std::stringstream cout;

    private:
        std::string TypeToPrefix(LOG_TYPE type);
        std::string TypeToColor(LOG_TYPE type);

        bool IsInitializedSanityCheck();

        std::string identifier;
        std::string additional_information = "";
        LOG_TYPE type = LOG_TYPE::LOG;

        bool isInitialized = false;
    };
}
