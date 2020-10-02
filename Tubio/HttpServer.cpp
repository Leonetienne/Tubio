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

		// Get clients ip address
		std::string peer_addr;
		{
			char buf[32];
			mg_sock_addr_to_str(&pNc->sa, buf, sizeof(buf), MG_SOCK_STRINGIFY_IP);
			peer_addr = buf;
		}

		std::string denialReason;
		if (IsConnectionAllowed(peer_addr, denialReason))
		{
			try
			{
				if (requestedUri == "/api")
				{
					ProcessAPIRequest(pNc, ev, p, peer_addr);
				}
				else if (requestedUri.substr(0, 9) == "/download")
				{
					ServeDownloadeableResource(pNc, ev, p, requestedUri);
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
		else // Client is not allowed, serve error json
		{
			Json j;
			j.CloneFrom(RestResponseTemplates::GetByCode(UNAUTHORIZED, denialReason));
			ServeStringToConnection(pNc, j.Render(), UNAUTHORIZED);
		}
	}

	return;
}

void HttpServer::ProcessAPIRequest(mg_connection* pNc, int ev, void* p, std::string peerAddress)
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

		

		JsonBlock responseBody;
		HTTP_STATUS_CODE returnCode;
		RestQueryHandler::ProcessQuery(peerAddress, requestBody, responseBody, returnCode);

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

void HttpServer::ServeDownloadeableResource(mg_connection* pNc, int ev, void* p, std::string uri)
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
		j.CloneFrom(RestResponseTemplates::GetByCode(BAD_REQUEST, "Invalid tubio id!"));
		ServeStringToConnection(pNc, j.Render(), BAD_REQUEST);
	}

	return;
}

bool HttpServer::IsConnectionAllowed(std::string peer_address, std::string& denialReason)
{
	// Localhost is always allowed!
	if (peer_address == "127.0.0.1") return true;

	// Peer is not localhost, but only localhost is allowed!
	else if (XGConfig::access.only_allow_localhost)
	{
		denialReason = "Only localhost allowed!";
		return false;
	}

	// Let's check if the whitelist is active
	else if (XGConfig::access.enable_whitelist)
	{
		// It is. Let's check if our peer is whitelisted
		for (std::size_t i = 0; i < XGConfig::access.whitelist.size(); i++)
		{
			// Whitelist is enabled, and peer is whitelisted
			if (XGConfig::access.whitelist[i] == peer_address) return true;
		}

		// Whitelist is enabled, but peer is NOT whitelisted
		denialReason = "Not whitelisted!";
		return false;
	}
	else // Whitelist is NOT enabled and only_allow_localhost is FALSE!
	{
		return true;
	}

	// Should never come to this point
	denialReason = "Access denied";
	return false;
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
