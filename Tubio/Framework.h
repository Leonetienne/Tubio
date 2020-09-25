#pragma once
#include "Logger.h"
#include "LogHistory.h"
#include "HttpServer.h"
#include "DownloadManager.h"
#include "XGControl.h"
#include "XGConfig.h"

class Framework
{
public:
    Framework();
    ~Framework();
    void Run();

private:
    void PostInit();
    void OnExit();
    void PreInit();
    void PostExit();

    Rest::HttpServer* httpServer;

    Logging::Logger* log;

    bool isRunning = true;
};

