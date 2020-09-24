#include "Framework.h"

using namespace Logging;
using namespace Rest;

Framework::Framework()
{
    PreInit();

    log = new Logger("Framework");
    log->cout << "Starting Tubio server...";
    log->Flush();
    restInterface = new RestInterface();

    PostInit();

    XGControl::keepServerRunning = true;
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
    while (XGControl::keepServerRunning)
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
    RestQueryHandler::PreInit();

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
    RestQueryHandler::PostExit();

    return;
}
