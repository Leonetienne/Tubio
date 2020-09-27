#pragma once
#include <vector>
#include <iostream>
#include <sstream>
#include "external_dependencies/casenta/mongoose/mongoose.h"
#include "Logger.h"
#include "RestResponseTemplates.h"
#include "RestQueryHandler.h"
#include "XGConfig.h"

namespace Rest
{
	class HttpServer
	{
	public:
		HttpServer();
		~HttpServer();

		void PostInit();
		void Update();
		void OnExit();

	private:
		bool InitWebServer();
		static void ProcessAPIRequest(struct mg_connection* pNc, int ev, void* p);
		static void ServeDownloadeableResource(struct mg_connection* pNc, int ev, void* p, std::string uri);

		static void EventHandler(struct mg_connection* pNc, int ev, void* p);
		static void ServeStringToConnection(struct mg_connection* c, std::string str, int httpStatusCode = 200);
		static std::string FixUnterminatedString(const char* cstr, const std::size_t len);


		struct mg_mgr* pMgr;
		struct mg_connection* pNc;
		static mg_serve_http_opts frontend_serve_opts;

		Logging::Logger* log;

		bool isBootedSuccessfully = false;
	};
}
