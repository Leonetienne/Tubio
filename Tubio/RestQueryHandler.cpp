#include "RestQueryHandler.h"

using namespace Rest;
using namespace Logging;
using namespace JasonPP;

void RestQueryHandler::PreInit()
{
	log = new Logger("RestQueryHandler");

	return;
}

bool RestQueryHandler::ProcessQuery(const std::string clientAdress, const Json& request, JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	log->SetAdditionalInformation(std::string("@") + clientAdress);

	if (!ValidateField("request", JSON_DATA_TYPE::STRING, request, responseBody))
	{
		responseCode = BAD_REQUEST;
		return false;
	}
	JsonBlock requestBody = request.AsJson;
	std::string requestName = requestBody.Get("request").AsString;
	
	if (requestName == "kill_yourself") return KillYourself(requestBody, responseBody, responseCode);
	
	
	
	responseBody.CloneFrom(RestResponseTemplates::GetByCode(NOT_FOUND, "The requested request was not found."));
	responseCode = NOT_FOUND;
	return false;
}

void Rest::RestQueryHandler::PostExit()
{
	delete log;
	log = nullptr;

	return;
}

bool RestQueryHandler::KillYourself(const JsonBlock& request, JsonBlock& responseBody, HTTP_STATUS_CODE& responseCode)
{
	XGControl::keepServerRunning = false;

	log->cout << "Shutting down server upon rest request...";
	log->Flush();

	responseCode = OK;
	responseBody.CloneFrom(RestResponseTemplates::GetByCode(OK));
	responseBody.Set("message") = "Goodbye! :3";
	return true;
}

bool RestQueryHandler::ValidateField(const std::string name, const JasonPP::JSON_DATA_TYPE type, const JasonPP::Json& checkThat, JasonPP::JsonBlock& putErrorResponseHere)
{
	if (checkThat.GetDataType() != JSON_DATA_TYPE::JSON)
	{
		putErrorResponseHere = RestResponseTemplates::GetByCode(BAD_REQUEST, "The request body must be a json struct! No json array or similar...");
		return false;
	}

	const JsonBlock& cachedJson = checkThat.AsJson;

	if (cachedJson.DoesShorthandExist(name))
	{
		const JsonData& cached = cachedJson.ShorthandGet(name);

		if ((cached.GetDataType() == type) ||
			((cached.IsOfNumericType()) && (type == JSON_DATA_TYPE::INT)) ||
			((cached.IsOfNumericType()) && (type == JSON_DATA_TYPE::FLOAT)))
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