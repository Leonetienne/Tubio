#pragma once
#include "external_dependencies/leonetienne/JasonPP/JasonPP.hpp"
#include "RestResponseTemplates.h"
#include "XGControl.h"
#include "Logger.h"
#include "DownloadManager.h"
#include "ConsoleManager.h"

namespace Rest
{
	class RestQueryHandler
	{
	public:
		static void PreInit();

		static bool ProcessQuery(const std::string clientAdress, const JasonPP::Json& request, JasonPP::JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode);

		static void PostExit();

	private:
		static bool Example_Foo(const JasonPP::JsonBlock& request, JasonPP::JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode);
		static bool FetchSessionCache(const JasonPP::JsonBlock& request, JasonPP::JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode);
		static bool FetchAlltimeCache(const JasonPP::JsonBlock& request, JasonPP::JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode);
		static bool QueueDownload(const JasonPP::JsonBlock& request, JasonPP::JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode);
		static bool ClearDownloadCache(const JasonPP::JsonBlock& request, JasonPP::JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode);
		static bool KillYourself(const JasonPP::JsonBlock& request, JasonPP::JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode);
		static bool HideConsole(const JasonPP::JsonBlock& request, JasonPP::JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode);
		static bool ShowConsole(const JasonPP::JsonBlock& request, JasonPP::JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode);
		static bool GetOSName(const JasonPP::JsonBlock& request, JasonPP::JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode);
		static bool FetchAlltimeLogs(const JasonPP::JsonBlock& request, JasonPP::JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode);
		static bool FetchSessionLogs(const JasonPP::JsonBlock& request, JasonPP::JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode);
		static bool GetDiskUsage(const JasonPP::JsonBlock& request, JasonPP::JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode);
		
		static bool ValidateField(const std::string name, const JasonPP::JDType type, const JasonPP::Json& checkThat, JasonPP::JsonBlock& putErrorResponseHere);
	
		static Logging::Logger* log;
	};
}
