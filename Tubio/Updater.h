#pragma once
#ifdef _WIN
#include <string>
#include <urlmon.h>
#include <Windows.h>
#include "external_dependencies/leonetienne/JasonPP/JasonPP.hpp" // We need Internal::Helpers
#endif

class Updater
{
public:
	/// <summary>
	/// Will update youtube-dl.exe on windows only!! Returns error message. On linux, you have to update it yourself, since it is a package of its own!
	/// </summary>
	/// <returns></returns>
	static std::string UpdateYoutubeDL();
};
