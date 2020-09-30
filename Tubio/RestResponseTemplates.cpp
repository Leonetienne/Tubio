#include "RestResponseTemplates.h"

using namespace JasonPP;

/*
Every query MUST have a value for "status"
Every query with status = "ERROR" MUST have a value for "description" and "message"!
*/

JsonBlock Rest::RestResponseTemplates::GetByCode(HTTP_STATUS_CODE code, std::string message)
{
	switch (code)
	{
	case HTTP_STATUS_CODE::OK:
		return OK();
	case HTTP_STATUS_CODE::BAD_REQUEST:
		return BadRequest(message);
	case HTTP_STATUS_CODE::FORBIDDEN:
		return Forbidden((message.length() > 0) ? message : "Forbidden!");
	case HTTP_STATUS_CODE::UNAUTHORIZED:
		return Unauthorized((message.length() > 0) ? message : "Unauthorized");
	case HTTP_STATUS_CODE::NOT_FOUND:
		return NotFound((message.length() > 0) ? message : "not found");
	case HTTP_STATUS_CODE::INTERNAL_SERVER_ERROR:
		return InternalServerError((message.length() > 0) ? message : "Well, that's not good.");
	case HTTP_STATUS_CODE::NOT_IMPLEMENTED:
		return NotImplemented(message);
	}
	return InternalServerError("No template found for this http-status-code");
}

JsonBlock Rest::RestResponseTemplates::OK()
{
	return JsonBlock({
		Ele("status", "OK"),
	});
}

JsonBlock Rest::RestResponseTemplates::BadRequest(std::string message)
{
	return JsonBlock({
		Ele("status", "ERROR"),
		Ele("description", "Bad request"),
		Ele("message", message)
	});
}

JsonBlock Rest::RestResponseTemplates::Unauthorized(std::string message)
{
	return JsonBlock({
		Ele("status", "ERROR"),
		Ele("description", "Unauthorized"),
		Ele("message", message)
	});
}

JsonBlock Rest::RestResponseTemplates::Forbidden(std::string message)
{
	return JsonBlock({
		Ele("status", "ERROR"),
		Ele("description", "Forbidden"),
		Ele("message", message)
	});
}

JsonBlock Rest::RestResponseTemplates::NotFound(std::string message)
{
	return JsonBlock({
		Ele("status", "ERROR"),
		Ele("description", "Not found"),
		Ele("message", message)
	});
}

JsonBlock Rest::RestResponseTemplates::InternalServerError(std::string message)
{
	return JsonBlock({
		Ele("status", "ERROR"),
		Ele("description", "Internal server error"),
		Ele("message", message)
	});
}

JsonBlock Rest::RestResponseTemplates::NotImplemented(std::string message)
{
	return JsonBlock({
		Ele("status", "ERROR"),
		Ele("description", "Not implemented"),
		Ele("message", message)
	});
}
