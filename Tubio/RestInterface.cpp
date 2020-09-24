#include "RestInterface.h"

using namespace Logging;
using namespace Rest;
using namespace JasonPP;

RestInterface::RestInterface()
{
	pMgr = new mg_mgr();
	pNc = nullptr;
	log = new Logger("WebServer");

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

	log->cout << "Starting rest api server on port " << WEBAPI_SERVER_PORT << "...";
	log->Flush();

	pNc = mg_bind(pMgr, WEBAPI_SERVER_PORT, this->EventHandler);

	if (pNc == NULL)
	{
		log->cout << log->Err() << "Failed to boot rest api web server! - Unable to bind listener! (port: " << WEBAPI_SERVER_PORT << ")";
		log->Flush();
		return false;
	}

	mg_set_protocol_http_websocket(pNc);
	frontend_serve_opts.document_root = "frontend";
	frontend_serve_opts.enable_directory_listing = "no";

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

void RestInterface::ServeStringToConnection(struct mg_connection* c, std::string str, int httpStatusCode)
{
	mg_send_head(c, httpStatusCode, str.length(), "content-type: application/json\nAccess-Control-Allow-Origin: *");
	mg_printf(c, str.c_str());

	return;
}

void RestInterface::EventHandler(mg_connection* pNc, int ev, void* p)
{
	switch (ev)
	{
	case MG_EV_HTTP_REQUEST:

		http_message* hpm = (http_message*)p;
		std::string requestedUri = FixUnterminatedString(hpm->uri.p, hpm->uri.len);
		
		if ((requestedUri == "/api"))
		{
			ProcessAPIRequest(pNc, ev, p);
		}
		else
		{
			mg_serve_http(pNc, (struct http_message*)p, frontend_serve_opts);
		}


		break;
	}

	return;
}

void RestInterface::ProcessAPIRequest(mg_connection* pNc, int ev, void* p)
{
	// Get struct with http message informations
	http_message* hpm = (http_message*)p;

	// Get the transmitted message body
	std::string requestBodyRaw = FixUnterminatedString(hpm->body.p, hpm->body.len);

	// Check for the body being valid json
	if (IsJsonValid(requestBodyRaw))
	{
		Json requestBody;
		requestBody.Parse(requestBodyRaw);

		char addr[32];
		mg_sock_addr_to_str(&pNc->sa, addr, sizeof(addr), MG_SOCK_STRINGIFY_IP);

		JsonBlock responseBody;
		HTTP_STATUS_CODE returnCode;
		RestQueryHandler::ProcessQuery(std::string(addr), requestBody, responseBody, returnCode);

		Json response(responseBody);
		ServeStringToConnection(pNc, response.Render(), returnCode);
	}
	else // return error message for invalid json
	{
		Json errorJson = RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::BAD_REQUEST, "Received json is fucked up");
		ServeStringToConnection(pNc, errorJson.Render(), HTTP_STATUS_CODE::BAD_REQUEST);
	}

	return;
}

void RestInterface::OnExit()
{
	log->cout << "Shutting down rest api server...";
	log->Flush();

	mg_mgr_free(pMgr);

	return;
}


std::string RestInterface::FixUnterminatedString(const char* cstr, const std::size_t len)
{
	std::stringstream ss;
	for (std::size_t i = 0; i < len; i++)
	{
		ss << *(cstr + i);
	}

	return ss.str();
}

mg_serve_http_opts RestInterface::frontend_serve_opts;
