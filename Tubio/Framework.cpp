#include "Framework.h"

using namespace Logging;
using namespace Rest;

Framework::Framework()
{
    PreInit();

    log = new Logger("Framework");
    log->cout << "Starting Tubio server...";
    log->Flush();

    httpServer = new HttpServer();

    PostInit();

    XGControl::keepServerRunning = true;
    Run();

    return;
}

Framework::~Framework()
{
    delete httpServer;
    delete log;

    httpServer = nullptr;
    log = nullptr;

    return;
}

void Framework::Run()
{
    while (XGControl::keepServerRunning)
    {
        httpServer->Update();
    }

    OnExit();
    PostExit();


    return;
}


void Framework::PreInit()
{
    LogHistory::PreInit();
    XGConfig::PreInit();
    RestQueryHandler::PreInit();

    return;
}


void Framework::PostInit()
{
    httpServer->PostInit();

    return;
}

void Framework::OnExit()
{
    httpServer->OnExit();

    return;
}

void Framework::PostExit()
{
    XGConfig::PostExit();
    RestQueryHandler::PostExit();
    LogHistory::PostExit();

    return;
}
