#pragma once
#include "JasonPP.hpp"
#include "RestResponseTemplates.h"
#include "XGControl.h"
#include "Logger.h"
#include "DownloadManager.h"

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
		static bool FetchQueue(const JasonPP::JsonBlock& request, JasonPP::JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode);
		static bool QueueDownload(const JasonPP::JsonBlock& request, JasonPP::JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode);
		static bool ClearDownloadCache(const JasonPP::JsonBlock& request, JasonPP::JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode);
		static bool KillYourself(const JasonPP::JsonBlock& request, JasonPP::JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode);
		
		static bool ValidateField(const std::string name, const JasonPP::JDType type, const JasonPP::Json& checkThat, JasonPP::JsonBlock& putErrorResponseHere);
	
		static Logging::Logger* log;
	};
}