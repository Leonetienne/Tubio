#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include "LogTypes.h"
#include "LogHistory.h"

namespace Logging
{
    class Log
    {
    public:

    public:
        //Creates a Logger object. Pass an identifier such as MySQL
        Log(std::string _identifier);

        //Clears the buffered string and resets the log type to default
        void Clear();

        //Sets the buffered string
        void Set(std::string _str);

        //Prints the buffered string to the console and clears it
        std::string Flush();

        //Sets a custom log type
        std::string Type(LOG_TYPE _type);

        //Sets the log type to warning
        std::string Warn() { return Type(WARN); }

        //Sets the log type to error
        std::string Err() { return Type(ERR); }

        std::stringstream cout;

    private:
        std::string TypeToPrefix(LOG_TYPE _type);
        std::string TypeToColor(LOG_TYPE _type);

        bool IsInitializedSanityCheck();

        std::string identifier;
        std::string rawIdentifier;
        LOG_TYPE type = LOG;

        bool isInitialized = false;
    };
}
