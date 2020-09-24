#pragma once
#include <vector>
#include <iostream>
#include <sstream>
#include "mongoose.h"
#include "Logger.h"
#include "RestResponseTemplates.h"
#include "RestQueryHandler.h"

#define WEBAPI_SERVER_POLLRATE 100
#define WEBAPI_SERVER_PORT "6969"

namespace Rest
{
	class RestInterface
	{
	public:
		RestInterface();
		~RestInterface();

		void PostInit();
		void Update();
		void OnExit();

	private:
		bool InitWebServer();

		static void EventHandler(struct mg_connection* pNc, int ev, void* p);
		static void ServeStringToConnection(struct mg_connection* c, std::string str, int httpStatusCode = 200);
		static std::string FixUnterminatedString(const char* cstr, const std::size_t len);


		struct mg_mgr* pMgr;
		struct mg_connection* pNc;

		Logging::Logger* log;

		bool isBootedSuccessfully = false;
	};
}
