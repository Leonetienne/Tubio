#include "RestQueryHandler.h"

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
	log->SetAdditionalInformation(std::string("@") + clientAdress);

	if (!ValidateField("request", JDType::STRING, request, responseBody))
	{
		responseCode = BAD_REQUEST;
		return false;
	}
	JsonBlock requestBody = request.AsJson;
	std::string requestName = requestBody.Get("request").AsString;
	


	if (requestName == "kill_yourself") return KillYourself(requestBody, responseBody, responseCode);
	else if (requestName == "queue_download") return QueueDownload(requestBody, responseBody, responseCode);
	else if (requestName == "fetch_queue") return FetchQueue(requestBody, responseBody, responseCode);
	else if (requestName == "clear_download_cache") return ClearDownloadCache(requestBody, responseBody, responseCode);
	else if (requestName == "foo") return Example_Foo(requestBody, responseBody, responseCode);
	else if (requestName == "show_console") return ShowConsole(requestBody, responseBody, responseCode);
	else if (requestName == "hide_console") return HideConsole(requestBody, responseBody, responseCode);
	else if (requestName == "get_os_name") return GetOSName(requestBody, responseBody, responseCode);
	
	
	
	responseBody.CloneFrom(RestResponseTemplates::GetByCode(NOT_FOUND, "The requested request was not found."));
	responseCode = NOT_FOUND;
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
	responseCode = OK;
	responseBody.CloneFrom(RestResponseTemplates::GetByCode(OK));
	responseBody.Set("message") = "Bar!";
	std::cout << "Bar!" << std::endl;
	return true;
}

bool RestQueryHandler::QueueDownload(const JsonBlock& request, JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	if ((!ValidateField("video_url", JDType::STRING, request, responseBody)) ||
		(!ValidateField("mode", JDType::STRING, request, responseBody)))
	{
		responseCode = BAD_REQUEST;
		return false;
	}

	std::string modeParam = request.Get("mode").AsString;
	std::string videoUrl = request.Get("video_url").AsString;
	DOWNLOAD_MODE mode;
	if (modeParam == "video") mode = DOWNLOAD_MODE::VIDEO;
	else if (modeParam == "audio") mode = DOWNLOAD_MODE::AUDIO;
	else
	{
		responseCode = BAD_REQUEST;
		responseBody.CloneFrom(RestResponseTemplates::GetByCode(BAD_REQUEST, "Parameter 'mode' is of wrong value. Should be either 'video' or 'audio'."));
		return false;
	}

	log->cout << "Queued video \"" << videoUrl << "\"...";
	log->Flush();

	std::string tubId = DownloadManager::QueueDownload(videoUrl, mode);

	responseCode = OK;
	responseBody.CloneFrom(RestResponseTemplates::GetByCode(OK));
	responseBody.Set("message") = "Download queued!";
	responseBody.Set("queue_position") = (long long int)DownloadManager::GetQueueLength();
	responseBody.Set("tubio_id") = tubId;
	responseBody.Set("queue") = DownloadManager::GetQueueAsJson();
	return true;
}

bool RestQueryHandler::FetchQueue(const JsonBlock& request, JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	responseCode = OK;
	responseBody.CloneFrom(RestResponseTemplates::GetByCode(OK));
	responseBody.Set("queue") = DownloadManager::GetQueueAsJson();
	return true;
}

bool RestQueryHandler::ClearDownloadCache(const JsonBlock& request, JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	responseCode = OK;
	responseBody.CloneFrom(RestResponseTemplates::GetByCode(OK));
	DownloadManager::ClearDownloadCache();

	log->cout << "Clearing download cache...";
	log->Flush();

	return true;
}

bool RestQueryHandler::KillYourself(const JsonBlock& request, JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	XGControl::keepServerRunning = false;

	log->cout << "Shutting down server upon client request...";
	log->Flush();

	responseCode = OK;
	responseBody.CloneFrom(RestResponseTemplates::GetByCode(OK));
	responseBody.Set("message") = "Goodbye! :3";
	return true;
}

bool RestQueryHandler::HideConsole(const JsonBlock& request, JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	if (ConsoleManager::IsSupported())
	{
		bool didAnythingChange = ConsoleManager::HideConsole();
		responseCode = OK;
		responseBody.CloneFrom(RestResponseTemplates::GetByCode(OK));
		responseBody.Set("message") = (didAnythingChange) ? "Console is now hidden!" : "Console was already hidden!";
		return true;
	}
	else
	{
		responseCode = NOT_IMPLEMENTED;
		responseBody.CloneFrom(RestResponseTemplates::GetByCode(NOT_IMPLEMENTED));
		responseBody.Set("message") = "This feature is currently only supported on Windows! Make sure to compile with preprocessor directive _WIN!";
		return false;
	}
}

bool RestQueryHandler::ShowConsole(const JsonBlock& request, JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	if (ConsoleManager::IsSupported())
	{
		bool didAnythingChange = ConsoleManager::ShowConsole();
		responseCode = OK;
		responseBody.CloneFrom(RestResponseTemplates::GetByCode(OK));
		responseBody.Set("message") = (didAnythingChange) ? "Console is now shown!" : "Console was already shown!";
		return true;
	}
	else
	{
		responseCode = NOT_IMPLEMENTED;
		responseBody.CloneFrom(RestResponseTemplates::GetByCode(NOT_IMPLEMENTED));
		responseBody.Set("message") = "This feature is currently only supported on Windows! Make sure to compile with preprocessor directive _WIN!";
		return false;
	}
}

bool RestQueryHandler::GetOSName(const JsonBlock& request, JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	std::string osName = "other";
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
	responseCode = OK;
	responseBody.CloneFrom(RestResponseTemplates::GetByCode(OK));
	responseBody.Set("os_name") = osName;
	return true;
}














bool RestQueryHandler::ValidateField(const std::string name, const JasonPP::JDType type, const JasonPP::Json& checkThat, JasonPP::JsonBlock& putErrorResponseHere)
{
	if (checkThat.GetDataType() != JDType::JSON)
	{
		putErrorResponseHere = RestResponseTemplates::GetByCode(BAD_REQUEST, "The request body must be a json struct! No json array or similar...");
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

			putErrorResponseHere.CloneFrom(RestResponseTemplates::GetByCode(BAD_REQUEST, ss.str()));
		}
	}
	else
	{
		putErrorResponseHere.CloneFrom(RestResponseTemplates::GetByCode(BAD_REQUEST, std::string("Missing mandatory value '" + name + "'")));
		return false;
	}

	return false;
}

Logger* RestQueryHandler::log;
