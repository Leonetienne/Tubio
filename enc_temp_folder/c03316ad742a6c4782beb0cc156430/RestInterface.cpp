#include "RestInterface.h"

using namespace Logging;

RestInterface::RestInterface()
{
	pMgr = new mg_mgr();
	pNc = nullptr;
	log = new Logger("WebServer");

	isBootedSuccessfully = false;

	return;
}

RestInterface::~RestInterface()
{
	delete pMgr;
	delete log;

	log = nullptr;
	pMgr = nullptr;

	return;
}

void RestInterface::PostInit()
{
	isBootedSuccessfully = InitWebServer();


	return;
}

bool RestInterface::InitWebServer()
{
	mg_mgr_init(pMgr, NULL);

	log->cout << "Starting web server on port " << WEBAPI_SERVER_PORT << "...";
	log->Flush();

	pNc = mg_bind(pMgr, WEBAPI_SERVER_PORT, this->EventHandler);

	if (pNc == NULL)
	{
		log->cout << log->Err() << "Failed to boot the web server! - Unable to bind listener!";
		log->Flush();
		return false;
	}

	mg_set_protocol_http_websocket(pNc);

	log->cout << "Started web server successfully!";
	log->Flush();
	isBootedSuccessfully = true;

	return true;
}

void RestInterface::Update()
{
	mg_mgr_poll(pMgr, WEBAPI_SERVER_POLLRATE);

	return;
}

void RestInterface::ServeStringToConnection(struct mg_connection* _c, std::string _str)
{
	mg_send_head(_c, 200, _str.length(), "content-type: application/json\nAccess-Control-Allow-Origin: *");
	mg_printf(_c, _str.c_str());

	return;
}


void RestInterface::EventHandler(mg_connection* _pNc, int _ev, void* _p)
{
	switch (_ev)
	{
	case MG_EV_HTTP_REQUEST:

		/*
		StringParser sp(_pNc->recv_mbuf.buf);

		sp.Skip("GET /");
		std::string rawRequest = sp.ExtSeek(" HTTP/");
		std::string jsonQuery = StringTools::UrlDecode(rawRequest);

		std::string queryResult = RestAPIQueryHandler::ProcessQuery(jsonQuery);



		ServeStringToConnection(_pNc, queryResult);
		*/

		std::cout << _pNc->recv_mbuf.buf << std::endl;
		
		http_message* hpm = (http_message*)_p;
		
		std::cout << hpm << std::endl;
		ServeStringToConnection(_pNc, _pNc->recv_mbuf.buf);

		break;
	}

	return;
}

void RestInterface::OnExit()
{
	mg_mgr_free(pMgr);

	return;
}
