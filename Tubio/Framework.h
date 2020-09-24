#pragma once
#include "Logger.h"
#include "LogHistory.h"
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

    Logging::Logger* log;

    bool isRunning = true;
};

