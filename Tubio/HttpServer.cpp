#include "HttpServer.h"

using namespace Logging;
using namespace Rest;
using namespace JasonPP;

HttpServer::HttpServer()
{
	pMgr = new mg_mgr();
	pNc = nullptr;
	log = new Logger("HttpServer");

	return;
}

HttpServer::~HttpServer()
{
	delete pMgr;
	delete log;

	log = nullptr;
	pMgr = nullptr;

	return;
}

void HttpServer::PostInit()
{
	isBootedSuccessfully = InitWebServer();

	return;
}

bool HttpServer::InitWebServer()
{
	mg_mgr_init(pMgr, NULL);

	log->cout << "Starting http-server on port " << XGConfig::httpServer.port << "...";
	log->Flush();

	pNc = mg_bind(pMgr, XGConfig::httpServer.port.c_str(), this->EventHandler);

	if (pNc == NULL)
	{
		log->cout << log->Err() << "Failed to boot the http-server! - Unable to bind listener! (port: " << XGConfig::httpServer.port << ")";
		log->Flush();
		return false;
	}

	mg_set_protocol_http_websocket(pNc);
	frontend_serve_opts.document_root = XGConfig::httpServer.rootdir.c_str();
	frontend_serve_opts.enable_directory_listing = "no";

	log->cout << "Started http-server successfully!";
	log->Flush();
	isBootedSuccessfully = true;

	return true;
}

void HttpServer::Update()
{
	mg_mgr_poll(pMgr, XGConfig::httpServer.polling_rate);

	return;
}

void HttpServer::ServeStringToConnection(struct mg_connection* c, std::string str, int httpStatusCode)
{
	mg_send_head(c, httpStatusCode, str.length(), "content-type: application/json\nAccess-Control-Allow-Origin: *");
	mg_printf(c, str.c_str());

	return;
}

void HttpServer::EventHandler(mg_connection* pNc, int ev, void* p)
{
	switch (ev)
	{
	case MG_EV_HTTP_REQUEST:

		http_message* hpm = (http_message*)p;
		std::string requestedUri = FixUnterminatedString(hpm->uri.p, hpm->uri.len);
		
		try
		{
			if (requestedUri == "/api")
			{
				ProcessAPIRequest(pNc, ev, p);
			}
			else if (requestedUri.substr(0, 9) == "/download")
			{
				ServeDownloadedResource(pNc, ev, p, requestedUri);
			}
			else
			{
				// Just serve the files requested
				mg_serve_http(pNc, (struct http_message*)p, frontend_serve_opts);
			}
		}
		catch (std::exception& e)
		{
			Json j;
			j.CloneFrom(RestResponseTemplates::GetByCode(INTERNAL_SERVER_ERROR, e.what()));
			ServeStringToConnection(pNc, j.Render(), INTERNAL_SERVER_ERROR);
		}
		catch (...)
		{
			Json j;
			j.CloneFrom(RestResponseTemplates::GetByCode(INTERNAL_SERVER_ERROR, "Das not good"));
			ServeStringToConnection(pNc, j.Render(), INTERNAL_SERVER_ERROR);
		}

		break;
	}

	return;
}

void HttpServer::ProcessAPIRequest(mg_connection* pNc, int ev, void* p)
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

		char peer_addr[32];
		mg_sock_addr_to_str(&pNc->sa, peer_addr, sizeof(peer_addr), MG_SOCK_STRINGIFY_IP);

		JsonBlock responseBody;
		HTTP_STATUS_CODE returnCode;
		RestQueryHandler::ProcessQuery(std::string(peer_addr), requestBody, responseBody, returnCode);

		Json response(responseBody);
		ServeStringToConnection(pNc, response.Render(), returnCode);
	}
	else // return error message for invalid json
	{
		Json errorJson;
		errorJson.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::BAD_REQUEST, "Received json is fucked"));
		ServeStringToConnection(pNc, errorJson.Render(), HTTP_STATUS_CODE::BAD_REQUEST);
	}

	return;
}

void HttpServer::ServeDownloadedResource(mg_connection* pNc, int ev, void* p, std::string uri)
{
	std::string fileId = uri.substr(10, uri.length() - 10);

	if (Downloader::DownloadManager::DoesTubioIDExist(fileId))
	{
		Downloader::DownloadEntry& entry = Downloader::DownloadManager::GetDownloadEntryByTubioID(fileId);
		
		if (entry.status == Downloader::DOWNLOAD_STATUS::FINISHED)
		{
			std::stringstream ss;
			std::string downloadedFilename = entry.title + (entry.mode == Downloader::DOWNLOAD_MODE::AUDIO ? ".mp3" : ".mp4");

			ss << "Access-Control-Allow-Origin: *\nContent-Disposition: attachment; filename=\"" << downloadedFilename << "\"\nPragma: public\nCache-Control: must-revalidate, post-check=0, pre-check=0";

			mg_http_serve_file(pNc, (http_message*)p, entry.downloaded_filename.c_str(), mg_mk_str("application/octet-stream"), mg_mk_str(ss.str().c_str()));
		}
		else
		{
			Json j;
			j.CloneFrom(RestResponseTemplates::GetByCode(BAD_REQUEST, "File download not ready!"));
			ServeStringToConnection(pNc, j.Render(), BAD_REQUEST);
		}
	}
	else
	{
		Json j;
		j.CloneFrom(RestResponseTemplates::GetByCode(BAD_REQUEST, "Invalid file id!"));
		ServeStringToConnection(pNc, j.Render(), BAD_REQUEST);
	}

	return;
}

void HttpServer::OnExit()
{
	log->cout << "Shutting down http-server...";
	log->Flush();

	mg_mgr_free(pMgr);

	return;
}


std::string HttpServer::FixUnterminatedString(const char* cstr, const std::size_t len)
{
	std::stringstream ss;
	for (std::size_t i = 0; i < len; i++)
	{
		ss << *(cstr + i);
	}

	return ss.str();
}

mg_serve_http_opts HttpServer::frontend_serve_opts;
