#pragma once
#include "Logger.h"
#include "LogHistory.h"
#include "HttpServer.h"
#include "XGControl.h"
#include "XGConfig.h"

class Framework
{
public:
    Framework();
    ~Framework();

private:
    void Run();

    void PostInit();
    void OnExit();
    void PreInit();
    void PostExit();

    Rest::HttpServer* httpServer;

    Logging::Logger* log;

    bool isRunning = true;
};

