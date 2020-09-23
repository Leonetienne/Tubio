#include "Framework.h"

using namespace Logging;

Framework::Framework()
{
    PreInit();

    log = new Logger("Framework");
    log->cout << "Starting Tubio server...";
    log->Flush();
    restInterface = new RestInterface();

    PostInit();

    isRunning = true;
    Run();

    return;
}

Framework::~Framework()
{
    delete restInterface;
    delete log;

    restInterface = nullptr;
    log = nullptr;

    return;
}

void Framework::Run()
{
    while (isRunning)
    {
        restInterface->Update();
    }

    OnExit();
    PostExit();


    return;
}


void Framework::PreInit()
{
    LogHistory::PreInit();

    return;
}


void Framework::PostInit()
{
    restInterface->PostInit();

    return;
}

void Framework::OnExit()
{
    restInterface->OnExit();

    return;
}

void Framework::PostExit()
{
    LogHistory::PostExit();

    return;
}
