#include "Framework.h"

using namespace Logging;
using namespace Rest;
using namespace Downloader;

Framework::Framework()
{
    ConsoleManager::PrePreInit();

    PreInit();

    log = new Logger("Framework");
    log->cout << "Starting Tubio server...";
    log->Flush();

    LogHistory::Init();

    httpServer = new HttpServer();

    PostInit();

    XGControl::keepServerRunning = true;

    log = new Logger("Framework");
    log->cout << "Started Tubio server successfully!";
    log->Flush();

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
        Update();
    }

    OnExit();
    PostExit();


    return;
}


void Framework::PreInit()
{
    LogHistory::PreInit();
    XGConfig::PreInit();
    ConsoleManager::PreInit();
    RestQueryHandler::PreInit();
    DownloadManager::PreInit();

    return;
}


void Framework::PostInit()
{
    ConsoleManager::PostInit();
    httpServer->PostInit();

    return;
}

void Framework::Update()
{
    httpServer->Update();
    DownloadManager::Update();
    LogHistory::Update();
    Idler::Update();

    return;
}

void Framework::OnExit()
{
    httpServer->OnExit();
    DownloadManager::OnExit();

    return;
}

void Framework::PostExit()
{
    XGConfig::PostExit();
    RestQueryHandler::PostExit();
    DownloadManager::PostExit();
    ConsoleManager::PostExit();
    LogHistory::PostExit();

    return;
}
