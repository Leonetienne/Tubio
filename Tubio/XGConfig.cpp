#include "XGConfig.h"

using namespace JasonPP;

void XGConfig::PreInit()
{
	log = new ::Logging::Logger("Config");

	LoadDefaultValues();

	if (FileSystem::Exists(XGCONFIG_FILE))
	{
		Load();
		log->cout << "Loaded config file";
		log->Flush();
	}
	else
	{
		// Save default config values, and thus, create a default config file
		Save();
		log->cout << log->Warn() << "Config file not found. Created default one.";
		log->Flush();
	}

	return;
}

bool XGConfig::IsJsonFieldValid(const JsonBlock& json, const std::string key, const JDType type)
{
	return ((json.DoesShorthandExist(key)) && (json.ShorthandGet(key).GetDataType() == type));
}

void XGConfig::LoadDefaultValues()
{
	httpServer.port = "80";
	httpServer.polling_rate = 100;
	httpServer.rootdir = "frontend";

	logging.logfile_text = "log.txt";
	logging.logfile_json = "log.json";
	logging.autosave_interval = 60 * 30; // 30 mins

	downloader.cachedir = "dlcache";
	downloader.max_dlrate_per_thread = "100M";
	downloader.num_threads = 1;

	general.show_console = true;

	access.only_allow_localhost = false;
	access.enable_whitelist = false;
	access.whitelist = std::vector<std::string>();
	access.whitelist.push_back("127.0.0.1"); // Add localhost to whitelist

	savefileBuffer.Clear();
	savefileBuffer.CloneFrom(CreateJson());

	return;
}

void XGConfig::LoadFromJson(const JasonPP::JsonBlock& json)
{
	savefileBuffer.Clear();
	savefileBuffer.CloneFrom(json);

	if (IsJsonFieldValid(json, "logging.logfile_json", JDType::STRING))
	{
		logging.logfile_json = json.ShorthandGet("logging.logfile_json").AsString;
	}

	if (IsJsonFieldValid(json, "logging.logfile_text", JDType::STRING))
	{
		logging.logfile_text = json.ShorthandGet("logging.logfile_text").AsString;
	}

	if (IsJsonFieldValid(json, "logging.autosave_interval", JDType::INT))
	{
		logging.autosave_interval = (uint32_t)json.ShorthandGet("logging.autosave_interval").AsInt;
	}



	if (IsJsonFieldValid(json, "httpServer.port", JDType::STRING))
	{
		httpServer.port = json.ShorthandGet("httpServer.port").AsString;
	}

	if (IsJsonFieldValid(json, "httpServer.polling_rate", JDType::INT))
	{
		httpServer.polling_rate = (uint32_t)json.ShorthandGet("httpServer.polling_rate").AsInt;
	}

	if (IsJsonFieldValid(json, "httpServer.rootdir", JDType::STRING))
	{
		httpServer.rootdir = json.ShorthandGet("httpServer.rootdir").AsString;
	}



	if (IsJsonFieldValid(json, "downloader.cachedir", JDType::STRING))
	{
		downloader.cachedir = json.ShorthandGet("downloader.cachedir").AsString;
	}
	if (IsJsonFieldValid(json, "downloader.num_threads", JDType::INT))
	{
		downloader.num_threads = (uint32_t)json.ShorthandGet("downloader.num_threads").AsInt;
	}
	if (IsJsonFieldValid(json, "downloader.max_dlrate_per_thread", JDType::STRING))
	{
		downloader.max_dlrate_per_thread = json.ShorthandGet("downloader.max_dlrate_per_thread").AsString;
	}

	if (IsJsonFieldValid(json, "general.show_console", JDType::BOOL))
	{
		general.show_console = json.ShorthandGet("general.show_console").AsBool;
	}



	if (IsJsonFieldValid(json, "access.only_allow_localhost", JDType::BOOL))
	{
		access.only_allow_localhost = json.ShorthandGet("access.only_allow_localhost").AsBool;
	}
	if (IsJsonFieldValid(json, "access.enable_whitelist", JDType::BOOL))
	{
		access.enable_whitelist = json.ShorthandGet("access.enable_whitelist").AsBool;
	}
	if (IsJsonFieldValid(json, "access.whitelist", JDType::ARRAY))
	{
		const JsonArray& cachedArr = json.ShorthandGet("access.whitelist").AsArray;
		access.whitelist.clear();
		for (std::size_t i = 0; i < cachedArr.Size(); i++)
		{
			if (cachedArr[i].GetDataType() == JDType::STRING)
			{
				access.whitelist.push_back(cachedArr[i].AsString);
			}
		}
	}
	
	return;
}

JsonBlock XGConfig::CreateJson()
{
	JsonArray jsnArrWhitelist;
	jsnArrWhitelist.Add(access.whitelist);

	return JsonBlock({
		Ele("httpServer", JsonBlock({
			Ele("port", httpServer.port),
			Ele("polling_rate", httpServer.polling_rate),
			Ele("rootdir", httpServer.rootdir)
		})),
		Ele("logging", JsonBlock({
			Ele("logfile_text", logging.logfile_text),
			Ele("logfile_json", logging.logfile_json),
			Ele("autosave_interval", logging.autosave_interval)
		})),
		Ele("downloader", JsonBlock({
			Ele("cachedir", downloader.cachedir),
			Ele("max_dlrate_per_thread", downloader.max_dlrate_per_thread),
			Ele("num_threads", downloader.num_threads),
		})),
		Ele("general", JsonBlock({
			Ele("show_console", general.show_console),
		})),
		Ele("access", JsonBlock({
			Ele("only_allow_localhost", access.only_allow_localhost),
			Ele("enable_whitelist", access.enable_whitelist),
			Ele("whitelist", jsnArrWhitelist),
		}))
	});
}

void XGConfig::PostExit()
{
	Save();

	delete log;

	log = nullptr;
	return;
}

void XGConfig::Save()
{
	/*
	All this shit is basically:
	-- Create backup of known good config
	-- Save new config
	-- Is new config parsable?
		-- Yes
			-- Delete backup
		-- No
			-- Restore backup
	*/

	log->cout << "Saving config...";
	log->Flush();


	if (!FileSystem::Copy(XGCONFIG_FILE, XGCONFIG_FILE_TMPBACKUP)) // Copy known-good file
	{
		log->cout << log->Err() << "Unable to create backup of config file! (backup is \"" << XGCONFIG_FILE_TMPBACKUP << "\")";
		log->Flush();
		return;
	}

	if (!SaveToFile(XGCONFIG_FILE)) // Now overwrite original file
	{
		log->cout << log->Err() << "Unable to save config file \"" << XGCONFIG_FILE_TMPBACKUP << "\"";
		log->Flush();
		return;
	}

	std::string writtenCode = FileSystem::ReadFile(XGCONFIG_FILE);
	if (IsJsonValid(writtenCode))
	{
		// All good, was successfully saved. Now delete the backup file.
		FileSystem::Delete(std::string(XGCONFIG_FILE) + "___");
	}
	else
	{
		// Saving was not successful. Let's copy the backup back.
		if (FileSystem::Copy(XGCONFIG_FILE_TMPBACKUP, XGCONFIG_FILE)) // Copy known-good file
		{
			log->cout << log->Err() << "Unable to restore backup of config file! (backup is \"" << XGCONFIG_FILE_TMPBACKUP << "\")";
			log->Flush();
			return;
		}

		if (FileSystem::Delete(XGCONFIG_FILE_TMPBACKUP))
		{
			log->cout << log->Err() << "Unable to delete backup of config file! (backup is \"" << XGCONFIG_FILE_TMPBACKUP << "\")";
			log->Flush();
			return;
		}

		log->cout << log->Warn() << "Unable to save config file to \"" << XGCONFIG_FILE << "\"!";
		log->Flush();
	}

	return;
}

bool XGConfig::SaveToFile(std::string filename)
{
	Json cfgStruct(CreateJson());
	savefileBuffer.Clear();
	savefileBuffer.CloneFrom(cfgStruct.AsJson);
	return FileSystem::WriteFile(filename, cfgStruct.Render());
}

void XGConfig::Load()
{
	std::string config_code = FileSystem::ReadFile(XGCONFIG_FILE);

	if (IsJsonValid(config_code))
	{
		Json json;
		json.Parse(config_code);

		if (json.GetDataType() == JDType::JSON)
		{
			LoadFromJson(json.AsJson);
		}
		else
		{
			log->cout << log->Warn() << "Unable to load config file because its content is not of json-type json!";
			log->Flush();
		}
	}
	else
	{
		log->cout << log->Warn() << "Unable to parse config file to \"" << XGCONFIG_FILE << "\" because it's json syntax is most likely fucked up!";
		log->Flush();
	}

	return;
}

XGConfig::HttpServer XGConfig::httpServer;
XGConfig::Logging XGConfig::logging;
XGConfig::Downloader XGConfig::downloader;
XGConfig::General XGConfig::general;
XGConfig::Access XGConfig::access;

JasonPP::JsonBlock XGConfig::savefileBuffer;
::Logging::Logger* XGConfig::log;
