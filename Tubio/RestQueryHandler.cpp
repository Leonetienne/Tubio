#include "RestQueryHandler.h"
#include "Version.h"

using namespace Rest;
using namespace Logging;
using namespace Downloader;
using namespace JasonPP;

void RestQueryHandler::PreInit()
{
	log = new Logger("RestQueryHandler");

	return;
}

bool RestQueryHandler::ProcessQuery(const std::string clientAdress, const Json& request, JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	// Print the client ip address alongide every log
	log->SetAdditionalInformation(std::string("@") + clientAdress);

	if (!ValidateField("request", JDType::STRING, request, responseBody))
	{
		responseCode = HTTP_STATUS_CODE::BAD_REQUEST;
		return false;
	}
	JsonBlock requestBody = request.AsJson;
	std::string requestName = requestBody.Get("request").AsString;
	


	if (requestName == "kill_yourself") return KillYourself(requestBody, responseBody, responseCode);
	else if (requestName == "queue_download") return QueueDownload(requestBody, responseBody, responseCode);
	else if (requestName == "fetch_session_cache") return FetchSessionCache(requestBody, responseBody, responseCode);
	else if (requestName == "fetch_alltime_cache") return FetchAlltimeCache(requestBody, responseBody, responseCode);
	else if (requestName == "get_disk_usage") return GetDiskUsage(requestBody, responseBody, responseCode);
	else if (requestName == "clear_download_cache") return ClearDownloadCache(requestBody, responseBody, responseCode);
	else if (requestName == "clear_logs") return ClearLogs(requestBody, responseBody, responseCode);
	else if (requestName == "foo") return Example_Foo(requestBody, responseBody, responseCode);
	else if (requestName == "show_console") return ShowConsole(requestBody, responseBody, responseCode);
	else if (requestName == "hide_console") return HideConsole(requestBody, responseBody, responseCode);
	else if (requestName == "get_os_name") return GetOSName(requestBody, responseBody, responseCode);
	else if (requestName == "fetch_session_logs") return FetchSessionLogs(requestBody, responseBody, responseCode);
	else if (requestName == "fetch_alltime_logs") return FetchAlltimeLogs(requestBody, responseBody, responseCode);
	else if (requestName == "update_dep_yt-dlp") return UpdateYtDlp(requestBody, responseBody, responseCode);
	else if (requestName == "remove_download_entry") return RemoveDownloadEntry(requestBody, responseBody, responseCode);
	else if (requestName == "update_config") return UpdateConfig(requestBody, responseBody, responseCode);
	else if (requestName == "reset_config_to_default_values") return ResetConfigDefaults(requestBody, responseBody, responseCode);
	else if (requestName == "get_server_version") return GetServerVersion(requestBody, responseBody, responseCode);
	
	
	
	responseBody.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::NOT_FOUND, "The requested request was not found."));
	responseCode = HTTP_STATUS_CODE::NOT_FOUND;
	return false;
}

void RestQueryHandler::PostExit()
{
	delete log;
	log = nullptr;

	return;
}

bool RestQueryHandler::Example_Foo(const JsonBlock& request, JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	responseCode = HTTP_STATUS_CODE::OK;
	responseBody.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::OK));
	responseBody.Set("message") = "Bar!";
	std::cout << "Bar!" << std::endl;
	return true;
}

bool RestQueryHandler::QueueDownload(const JsonBlock& request, JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	// Fetch parameters
	if ((!ValidateField("video_url", JDType::STRING, request, responseBody)) ||
		(!ValidateField("mode", JDType::STRING, request, responseBody)))
	{
		responseCode = HTTP_STATUS_CODE::BAD_REQUEST;
		return false;
	}

	std::string videoUrl = request.Get("video_url").AsString;
	std::string modeParam = request.Get("mode").AsString;
	std::string qualityParam;

	// 'quality' is an optional parameter. Default value is 'best'
	if ((ValidateField("quality", JDType::STRING, request, responseBody)))
	{
		qualityParam = request.Get("quality").AsString;
	}
	else
	{
		qualityParam = "best";
	}

	// Process parameters
	// Process download mode
	DOWNLOAD_MODE mode;
	if (modeParam == "video") mode = DOWNLOAD_MODE::VIDEO;
	else if (modeParam == "audio") mode = DOWNLOAD_MODE::AUDIO;
	else
	{
		responseCode = HTTP_STATUS_CODE::BAD_REQUEST;
		responseBody.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::BAD_REQUEST, "Parameter 'mode' is of wrong value. Should be either 'video' or 'audio'."));
		return false;
	}

	// Process download quality
	DOWNLOAD_QUALITY quality = DownloadManager::GetDownloadQualityByName(qualityParam);
	if (quality == DOWNLOAD_QUALITY::INVALID) {
		responseCode = HTTP_STATUS_CODE::BAD_REQUEST;
		responseBody.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::BAD_REQUEST, "Parameter 'quality' is of wrong value. Choose either 'best', '1440p', '1080p', '720p', '480p', '360p', '240p', or '144p'."));
		return false;
	}

	log->cout << "Queued video \"" << videoUrl << "\"...";
	log->Flush();

	std::string tubId = DownloadManager::QueueDownload(videoUrl, mode, quality);

	responseCode = HTTP_STATUS_CODE::OK;
	responseBody.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::OK));
	responseBody.Set("message") = "Download queued!";
	responseBody.Set("queue_position") = (long long int)DownloadManager::GetQueueLength();
	responseBody.Set("tubio_id") = tubId;
	JsonArray cache = DownloadManager::GetAlltimeCacheAsJson(time(0) - XGControl::boot_time, -1);
	responseBody.Set("cache_size") = (long long int)cache.Size();
	responseBody.Set("cache") = cache;
	return true;
}

bool RestQueryHandler::FetchSessionCache(const JsonBlock& request, JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	//log->cout << "Asking for session cache...";
	//log->Flush();

	time_t max_age = time(0) - XGControl::boot_time; // Default max_age is session length
	std::size_t max_num = (std::size_t )-1;			 // Default max_num is infinite

	JsonBlock dummy;
	if (ValidateField("max_age", JDType::INT, request, dummy))
	{
		// Only respect the given max_age, if the age does not exceed tubio boot-time!
		// This way we can ensure that only entries of this SESSION will appear!
		if (request["max_age"].AsInt >= 0)
		{
#ifdef _WIN
			max_age = min(request["max_age"].AsInt, max_age);
#else
			max_age = std::min<long int>(request["max_age"].AsInt, max_age);
#endif
		}
	}
	if (ValidateField("max_num", JDType::INT, request, dummy))
	{
		max_num = request["max_num"].AsInt;
	}

	responseCode = HTTP_STATUS_CODE::OK;
	responseBody.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::OK));
	JsonArray cache = DownloadManager::GetAlltimeCacheAsJson(max_age, max_num);
	responseBody.Set("cache_size") = (long long int)cache.Size();
	responseBody.Set("cache") = cache;
	return true;
}

bool RestQueryHandler::FetchAlltimeCache(const JsonBlock& request, JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	//log->cout << "Asking for whole cache...";
	//log->Flush();

	responseCode = HTTP_STATUS_CODE::OK;
	responseBody.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::OK));
	JsonArray cache = DownloadManager::GetAlltimeCacheAsJson(-1, -1); // Get ALL the data
	responseBody.Set("cache_size") = (long long int)cache.Size();
	responseBody.Set("cache") = cache;

	return true;
}

bool RestQueryHandler::ClearDownloadCache(const JsonBlock& request, JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	log->cout << "Clearing download cache...";
	log->Flush();

	bool wait = !DownloadManager::ClearDownloadCache();

	responseCode = HTTP_STATUS_CODE::OK;
	responseBody.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::OK));

	if (wait)
	{
		responseBody.Set("status") = "OK_WAIT";
		responseBody.Set("message") = "Download cache cannot be cleared right now because there are active downloads, but will be cleared as soon as those have finished!";
	}
	else
	{
		responseBody.Set("message") = "Download cache has been cleared!";
	}

	return true;
}

bool RestQueryHandler::KillYourself(const JsonBlock& request, JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	XGControl::keepServerRunning = false;

	log->cout << "Shutting down server upon client request...";
	log->Flush();

	responseCode = HTTP_STATUS_CODE::OK;
	responseBody.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::OK));
	responseBody.Set("message") = "Goodbye! :3";
	return true;
}

bool RestQueryHandler::HideConsole(const JsonBlock& request, JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	if (ConsoleManager::IsSupported())
	{
		log->cout << "Hiding console...";
		log->Flush();

		bool didAnythingChange = ConsoleManager::HideConsole();
		responseCode = HTTP_STATUS_CODE::OK;
		responseBody.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::OK));
		responseBody.Set("message") = (didAnythingChange) ? "Console is now hidden!" : "Console was already hidden!";
		return true;
	}
	else
	{
		responseCode = HTTP_STATUS_CODE::NOT_IMPLEMENTED;
		responseBody.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::NOT_IMPLEMENTED));
		responseBody.Set("message") = "This feature is currently only supported on Windows! Make sure to compile with preprocessor directive _WIN!";
		return false;
	}
}

bool RestQueryHandler::ShowConsole(const JsonBlock& request, JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	if (ConsoleManager::IsSupported())
	{
		log->cout << "Showing console...";
		log->Flush();

		bool didAnythingChange = ConsoleManager::ShowConsole();
		responseCode = HTTP_STATUS_CODE::OK;
		responseBody.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::OK));
		responseBody.Set("message") = (didAnythingChange) ? "Console is now shown!" : "Console was already shown!";
		return true;
	}
	else
	{
		responseCode = HTTP_STATUS_CODE::NOT_IMPLEMENTED;
		responseBody.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::NOT_IMPLEMENTED));
		responseBody.Set("message") = "This feature is currently only supported on Windows! Make sure to compile with preprocessor directive _WIN!";
		return false;
	}
}

bool RestQueryHandler::GetOSName(const JsonBlock& request, JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	log->cout << "Asking for server OS name...";
	log->Flush();

	std::string osName = "Other";
#ifdef _WIN
	osName = "Windows";
#elif __APPLE__ || __MACH__
	osName = "Mac OSX";
#elif __linux__
	osName = "Linux";
#elif __FreeBSD__
	osName = "FreeBSD";
#elif __unix || __unix__
	osName = "Unix";
#endif
	responseCode = HTTP_STATUS_CODE::OK;
	responseBody.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::OK));
	responseBody.Set("os_name") = osName;
	return true;
}

bool RestQueryHandler::FetchSessionLogs(const JsonBlock& request, JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	//log->cout << "Fetching session logs...";
	//log->Flush();

	responseCode = HTTP_STATUS_CODE::OK;
	responseBody.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::OK));
	JsonArray logs = LogHistory::GetCompleteLogHistoryAsJson(time(0) - XGControl::boot_time + 1, -1);
	responseBody.Set("logs_size") = (long long int)logs.Size();
	responseBody.Set("logs") = logs;
	return true;
}

bool RestQueryHandler::FetchAlltimeLogs(const JsonBlock& request, JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	//log->cout << "Fetching alltime logs...";
	//log->Flush();

	time_t max_age = -1;
	std::size_t max_num = (std::size_t) - 1;

	JsonBlock dummy;
	if (ValidateField("max_age", JDType::INT, request, dummy))
	{
		max_age = request["max_age"].AsInt;
	}
	if (ValidateField("max_num", JDType::INT, request, dummy))
	{
		max_num = request["max_num"].AsInt;
	}

	responseCode = HTTP_STATUS_CODE::OK;
	responseBody.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::OK));
	JsonArray logs = LogHistory::GetCompleteLogHistoryAsJson(max_age, max_num);
	responseBody.Set("logs_size") = (long long int)logs.Size();
	responseBody.Set("logs") = logs;
	return true;
}

bool RestQueryHandler::GetDiskUsage(const JsonBlock& request, JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	log->cout << "Fetching disk usage...";
	log->Flush();

	responseCode = HTTP_STATUS_CODE::OK;
	responseBody.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::OK));

	JsonBlock diskUsages;

	long long int dlcache = 0;
	if (FileSystem::ExistsDirectory(XGConfig::downloader.cachedir))
	{
		dlcache = FileSystem::CalculateSize(XGConfig::downloader.cachedir, true);
	}
	diskUsages.Set("dlcache") = dlcache;

	long long int logs = 0;
	if (FileSystem::Exists("log.txt"))
	{
		logs += FileSystem::CalculateSize("log.txt");
	}
	if (FileSystem::Exists("log.json"))
	{
		logs += FileSystem::CalculateSize("log.json");
	}
	diskUsages.Set("logs") = logs;

	long long int misc = 0;
	if (FileSystem::Exists("config.json"))
	{
		misc += FileSystem::CalculateSize("config.json");
	}
	diskUsages.Set("misc") = misc;

	long long int dependencies = 0;
	if (FileSystem::Exists("ffmpeg.exe"))
	{
		dependencies += FileSystem::CalculateSize("ffmpeg.exe");
	}
	if (FileSystem::Exists("ffprobe.exe"))
	{
		dependencies += FileSystem::CalculateSize("ffprobe.exe");
	}
	if (FileSystem::Exists("ffplay.exe"))
	{
		dependencies += FileSystem::CalculateSize("ffplay.exe");
	}
	if (FileSystem::Exists("yt-dlp.exe"))
	{
		dependencies += FileSystem::CalculateSize("yt-dlp.exe");
	}
	diskUsages.Set("dependencies") = dependencies;

	diskUsages.Set("total") = dlcache + logs + misc + dependencies;

	responseBody.Set("disk_usage") = diskUsages;
	responseBody.Set("message") = "Disk usage in bytes. Dependencies are 0 on linux, because the dependencies should be installed globally.";

	return true;
}

bool RestQueryHandler::ClearLogs(const JsonBlock& request, JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	responseCode = HTTP_STATUS_CODE::OK;
	responseBody.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::OK));
	responseBody.Set("message") = "The logs have been cleared.";

	LogHistory::ClearLogHistory();
	log->cout << "Cleared logs...";
	log->Flush();

	return true;
}

bool RestQueryHandler::UpdateYtDlp(const JsonBlock& request, JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	log->cout << "Updating yt-dlp...";
	log->Flush();

	std::string result = Updater::UpdateYtDlp();
	if (result == "OK")
	{
		log->cout << "   => OK!";
		log->Flush();

		responseCode = HTTP_STATUS_CODE::OK;
		responseBody.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::OK));
		responseBody.Set("message") = "Updated yt-dlp.exe successfully!";
	}
	else if (result == "not implemented")
	{
		log->cout << "   => NOT_IMPLEMENTED!";
		log->Flush();

		log->Flush();
		responseCode = HTTP_STATUS_CODE::NOT_IMPLEMENTED;
		responseBody.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::NOT_IMPLEMENTED));
		responseBody.Set("message") = "On linux you have to update yt-dlp yourself since it is a system-wide package handled by various package managers!";
	}
	else // Some other error
	{
		log->cout << "   => urlmon error: " << result;
		log->Flush();

		responseCode = HTTP_STATUS_CODE::INTERNAL_SERVER_ERROR;
		responseBody.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::INTERNAL_SERVER_ERROR));
		responseBody.Set("message") = "Unable do update yt-dlp.exe! See urlmon " + result;
	}
	
	return true;
}

bool RestQueryHandler::RemoveDownloadEntry(const JsonBlock& request, JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	if (!ValidateField("id", JDType::STRING, request, responseBody))
	{
		responseCode = HTTP_STATUS_CODE::BAD_REQUEST;
		return false;
	}

	log->cout << "Removing download id " << request["id"].AsString << "...";
	log->Flush();

	bool didSucceed = Downloader::DownloadManager::RemoveFromCacheByID(request["id"].AsString);

	if (didSucceed)
	{
		responseCode = HTTP_STATUS_CODE::OK;
		responseBody.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::OK));
		responseBody.Set("message") = "Successfully removed.";
	}
	else
	{
		responseCode = HTTP_STATUS_CODE::BAD_REQUEST;
		responseBody.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::BAD_REQUEST));
		responseBody.Set("message") = "Failed.";
	}

	return true;
}

bool RestQueryHandler::UpdateConfig(const JsonBlock& request, JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	responseCode = HTTP_STATUS_CODE::OK;
	responseBody.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::OK));

	JsonBlock dummy;
	if (ValidateField("config", JDType::JSON, request, dummy))
	{
		bool prevStateConsole = XGConfig::general.show_console;
		XGConfig::LoadFromJson(request.Get("config").AsJson);

		// Update console, if necessary
		if (XGConfig::general.show_console != prevStateConsole)
		{
			if (XGConfig::general.show_console) ConsoleManager::ShowConsole();
			else ConsoleManager::HideConsole();
		}

		log->cout << "Updated config values...";
		log->Flush();
		XGConfig::Save();

		responseBody.Set("message") = "Updated settings";
	}
	else
	{
		responseBody.Set("message") = "Updated no settings";
	}

	responseBody.Set("config") = XGConfig::GetSavefileBuffer();
	return true;
}

bool RestQueryHandler::ResetConfigDefaults(const JsonBlock& request, JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	log->cout << "Reset config values to default...";
	log->Flush();

	bool prevStateConsole = XGConfig::general.show_console;
	XGConfig::LoadDefaultValues();
	XGConfig::Save();
	// Update console, if necessary
	if (XGConfig::general.show_console != prevStateConsole)
	{
		if (XGConfig::general.show_console) ConsoleManager::ShowConsole();
		else ConsoleManager::HideConsole();
	}

	responseCode = HTTP_STATUS_CODE::OK;
	responseBody.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::OK));
	responseBody.Set("message") = "Reset config to default...";
	responseBody.Set("config") = XGConfig::GetSavefileBuffer();
	return true;
}

bool Rest::RestQueryHandler::GetServerVersion(const JasonPP::JsonBlock& request, JasonPP::JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	responseCode = HTTP_STATUS_CODE::OK;
	responseBody.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::OK));
	responseBody.Set("server_version").SetFloatData(TUBIO_SERVER_VERSION);
	return true;
}









bool RestQueryHandler::ValidateField(const std::string name, const JasonPP::JDType type, const JasonPP::Json& checkThat, JasonPP::JsonBlock& putErrorResponseHere)
{
	if (checkThat.GetDataType() != JDType::JSON)
	{
		putErrorResponseHere = RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::BAD_REQUEST, "The request body must be a json struct! No json array or similar...");
		return false;
	}

	const JsonBlock& cachedJson = checkThat.AsJson;

	if (cachedJson.DoesShorthandExist(name))
	{
		const JsonData& cached = cachedJson.ShorthandGet(name);

		if ((cached.GetDataType() == type) ||
			((cached.IsOfNumericType()) && (type == JDType::INT)) ||
			((cached.IsOfNumericType()) && (type == JDType::FLOAT)))
		{
			return true;
		}
		else
		{
			std::stringstream ss;
			ss << "Mandatory value \"" << name << "\" is of wrong type (" << JsonDataType2String(cached.GetDataType()) << ")" << std::endl;
			ss << "Should be of type " << JsonDataType2String(type) << "! (Integers can be casted to floats)";

			putErrorResponseHere.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::BAD_REQUEST, ss.str()));
		}
	}
	else
	{
		putErrorResponseHere.CloneFrom(RestResponseTemplates::GetByCode(HTTP_STATUS_CODE::BAD_REQUEST, std::string("Missing mandatory value '" + name + "'")));
		return false;
	}

	return false;
}

Logger* RestQueryHandler::log;
