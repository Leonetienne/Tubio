#pragma once
#include "JasonPP.hpp"

namespace Rest
{
	enum HTTP_STATUS_CODE {
		OK						= 200,
		BAD_REQUEST				= 400,
		UNAUTHORIZED			= 401,
		FORBIDDEN				= 403,
		NOT_FOUND				= 404,
		INTERNAL_SERVER_ERROR	= 500,
		NOT_IMPLEMENTED			= 501
	};

	class RestResponseTemplates
	{
	public:
		static JasonPP::JsonBlock GetByCode(HTTP_STATUS_CODE code, std::string message = "");

	private:
		static JasonPP::JsonBlock OK();
		static JasonPP::JsonBlock BadRequest(std::string message);
		static JasonPP::JsonBlock Unauthorized(std::string message);
		static JasonPP::JsonBlock Forbidden(std::string message);
		static JasonPP::JsonBlock NotFound(std::string message);

		static JasonPP::JsonBlock InternalServerError(std::string message);
		static JasonPP::JsonBlock NotImplemented(std::string message);
	};
}
