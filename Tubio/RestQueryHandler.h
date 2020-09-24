#pragma once
#include "JasonPP.hpp"
#include "RestResponseTemplates.h"
#include "XGControl.h"
#include "Logger.h"

namespace Rest
{
	class RestQueryHandler
	{
	public:
		static void PreInit();

		static bool ProcessQuery(const std::string clientAdress, const JasonPP::Json& request, JasonPP::JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode);

		static void PostExit();

	private:
		static bool KillYourself(const JasonPP::JsonBlock& request, JasonPP::JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode);
		
		static bool ValidateField(const std::string name, const JasonPP::JSON_DATA_TYPE type, const JasonPP::Json& checkThat, JasonPP::JsonBlock& putErrorResponseHere);
	
		static Logging::Logger* log;
	};
}
