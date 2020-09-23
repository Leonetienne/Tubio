#pragma once
#include <vector>
#include <iostream>
#include <sstream>
#include "mongoose.h"
#include "Logger.h"

#define WEBAPI_SERVER_POLLRATE 100
#define WEBAPI_SERVER_PORT "6969"

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

	static void EventHandler(struct mg_connection* _pNc, int _ev, void* _p);

	static void ServeStringToConnection(struct mg_connection* _c, std::string _str);


	struct mg_mgr* pMgr;
	struct mg_connection* pNc;
	static mg_serve_http_opts pServeOpts;

	Logging::Logger* log;

	bool isBootedSuccessfully;
};

