#pragma once
#include "XGConfig.h"
#include "Logger.h"

#ifdef _WIN
#include <Windows.h>
#endif

class ConsoleManager
{
public:
	static void PrePreInit();
	static void PreInit();
	static void PostInit();
	static void PostExit();

	static bool ShowConsole();
	static bool HideConsole();
	static bool IsConsoleShown();

	// Returns whether or not the current platform supports hiding the console
	static bool IsSupported();

private:
	static bool isConsoleActive;
	static Logging::Logger* log;
	
#ifdef _WIN
	static HWND consoleHandle;
#endif
};

