#pragma once
#include "Logger.h"
#include "LogHistory.h"
#include "RestInterface.h"

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

    RestInterface* restInterface;

    Log* log;

    bool isRunning = true;
};

