#include "Framework.h"

using namespace Logging;
using namespace Rest;
using namespace UI;

Framework::Framework()
{
    PreInit();

    log = new Logger("Framework");
    log->cout << "Starting Tubio server...";
    log->Flush();

    restInterface = new RestInterface();
    uiServer = new UIServer();

    PostInit();

    XGControl::keepServerRunning = true;
    Run();

    return;
}

Framework::~Framework()
{
    delete restInterface;
    delete uiServer;
    delete log;

    uiServer = nullptr;
    restInterface = nullptr;
    log = nullptr;

    return;
}

void Framework::Run()
{
    while (XGControl::keepServerRunning)
    {
        restInterface->Update();
        uiServer->Update();
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
    uiServer->PostInit();

    return;
}

void Framework::OnExit()
{
    restInterface->OnExit();
    uiServer->OnExit();

    return;
}

void Framework::PostExit()
{
    LogHistory::PostExit();
    RestQueryHandler::PostExit();

    return;
}
