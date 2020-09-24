#include "UIServer.h"

using namespace UI;
using namespace Logging;

UIServer::UIServer()
{
	pMgr = new mg_mgr();
	pNc = nullptr;
	log = new Logger("UIServer");

	return;
}

UIServer::~UIServer()
{
	delete pMgr;
	delete log;

	log = nullptr;
	pMgr = nullptr;

	return;
}

void UIServer::PostInit()
{
	isBootedSuccessfully = InitWebServer();

	return;
}

bool UIServer::InitWebServer()
{
	mg_mgr_init(pMgr, NULL);

	log->cout << "Starting ui web server on port " << WEBUI_SERVER_PORT << "...";
	log->Flush();

	pNc = mg_bind(pMgr, WEBUI_SERVER_PORT, this->EventHandler);

	if (pNc == NULL)
	{
		log->cout << log->Err() << "Failed to boot ui web server! - Unable to bind listener! (port: " << WEBUI_SERVER_PORT << ")";
		log->Flush();
		return false;
	}

	mg_set_protocol_http_websocket(pNc);
	serve_opts.document_root = "frontend";  // Serve current directory
	serve_opts.enable_directory_listing = "no";

	log->cout << "Started web server successfully!";
	log->Flush();
	isBootedSuccessfully = true;

	return true;
}


void UIServer::Update()
{
	mg_mgr_poll(pMgr, WEBUI_SERVER_POLLRATE);

	return;
}

void UIServer::EventHandler(mg_connection* pNc, int ev, void* p)
{
	switch (ev)
	{
	case MG_EV_HTTP_REQUEST:
		mg_serve_http(pNc, (struct http_message*)p, serve_opts);
		break;
	}

	return;
}


void UIServer::OnExit()
{
	log->cout << "Shutting down ui web server...";
	log->Flush();

	mg_mgr_free(pMgr);

	return;
}

mg_serve_http_opts UIServer::serve_opts;