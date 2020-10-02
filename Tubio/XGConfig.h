#pragma once
#include <vector>
#include "Filesystem.h"
#include "external_dependencies/leonetienne/JasonPP/JasonPP.hpp"
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
		int polling_rate;
	};
	struct Logging
	{
		std::string logfile_text;
		std::string logfile_json;
		int autosave_interval;
	};
	struct Downloader
	{
		std::string cachedir;
		std::string max_dlrate_per_thread;
		int num_threads;
	};
	struct General
	{
		bool show_console;
	};
	struct Access
	{
		bool only_allow_localhost;
		bool enable_whitelist;
		std::vector<std::string> whitelist;
	};

	static void PreInit();
	static void Save();
	static void PostExit();

	static void LoadFromJson(const JasonPP::JsonBlock& json);
	static void LoadDefaultValues();

	static XGConfig::HttpServer httpServer;
	static XGConfig::Logging logging;
	static XGConfig::Downloader downloader;
	static XGConfig::General general;
	static XGConfig::Access access;

	static const JasonPP::JsonBlock& GetSavefileBuffer() { return savefileBuffer; }

	static ::Logging::Logger* log;

private:
	static bool SaveToFile(std::string filename);
	static JasonPP::JsonBlock savefileBuffer;

	static bool IsJsonFieldValid(const JasonPP::JsonBlock& json, const std::string key, const JasonPP::JDType type);
	static JasonPP::JsonBlock CreateJson();
	static void Load();
};

