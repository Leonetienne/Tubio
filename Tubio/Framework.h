#pragma once
#include "Logger.h"
#include "LogHistory.h"
#include "UIServer.h"
#include "RestInterface.h"
#include "XGControl.h"

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

    Rest::RestInterface* restInterface;
    UI::UIServer* uiServer;

    Logging::Logger* log;

    bool isRunning = true;
};

