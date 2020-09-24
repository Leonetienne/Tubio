#pragma once
#include "mongoose.h"
#include "Logger.h"

#define WEBUI_SERVER_PORT "6970"
#define WEBUI_SERVER_POLLRATE 0

namespace UI
{
	class UIServer
	{
	public:
		UIServer();
		~UIServer();

		void PostInit();
		void Update();
		void OnExit();

	private:
		bool InitWebServer();

		static void EventHandler(struct mg_connection* pNc, int ev, void* p);

		struct mg_mgr* pMgr;
		struct mg_connection* pNc;
		static mg_serve_http_opts serve_opts;

		Logging::Logger* log;

		bool isBootedSuccessfully = false;
	};
}
