#pragma once
#include "Filesystem.h"
#include "JasonPP.hpp"
#include "Logger.h"

#define XGCONFIG_FILE "config.json"
#define XGCONFIG_FILE_TMPBACKUP (std::string(XGCONFIG_FILE) + "___")

class XGConfig
{
public:
	struct HttpServer
	{
		std::string port;
		std::string rootdir;
		int pollingRate;
	};
	struct Logging
	{
		std::string logfile_text;
		std::string logfile_json;
	};


	static void PreInit();
	static void Save();
	static void PostExit();

	static HttpServer httpServer;
	static XGConfig::Logging logging;

	static ::Logging::Logger* log;

private:
	static bool SaveToFile(std::string filename);

	static bool IsJsonFieldValid(const JasonPP::JsonBlock& json, const std::string key, const JasonPP::JDType type);
	static void InitializeDefaultValues();
	static JasonPP::JsonBlock CreateJson();
	static void LoadFromJson(const JasonPP::JsonBlock& json);
	static void Load();
};

