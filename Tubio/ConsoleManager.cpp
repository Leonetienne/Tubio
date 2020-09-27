#include "ConsoleManager.h"

void ConsoleManager::PrePreInit()
{
#ifdef _WIN
	consoleHandle = GetConsoleWindow();
#endif
	return;
}

void ConsoleManager::PreInit()
{
	log = new Logging::Logger("ConsoleManager");
	isConsoleActive = XGConfig::general.show_console;

	return;
}

void ConsoleManager::PostInit()
{
#ifdef _WIN
	if (isConsoleActive)
	{
		ShowWindow(consoleHandle, SW_SHOW);
		BringWindowToTop(consoleHandle);
	}
	else
	{
		ShowWindow(consoleHandle, SW_HIDE);
	}
#endif

	return;
}

bool ConsoleManager::ShowConsole()
{
	#ifdef _WIN
	if (!IsConsoleShown())
	{
		ShowWindow(consoleHandle, SW_SHOW);
		BringWindowToTop(consoleHandle);
		XGConfig::general.show_console = true;
		isConsoleActive = true;
		return true;
	}
	#endif
	return false;
}

bool ConsoleManager::HideConsole()
{
	#ifdef _WIN
	if (IsConsoleShown())
	{
		ShowWindow(consoleHandle, SW_HIDE);
		XGConfig::general.show_console = false;
		isConsoleActive = false;
		return true;
	}
	#endif
	return false;
}

bool ConsoleManager::IsConsoleShown()
{
	return isConsoleActive;
}

bool ConsoleManager::IsSupported()
{
#ifdef _WIN
	return true;
#endif
	return false;
}

void ConsoleManager::PostExit()
{
	delete log;
	log = nullptr;

	return;
}


bool ConsoleManager::isConsoleActive;
Logging::Logger* ConsoleManager::log;

#ifdef _WIN
HWND ConsoleManager::consoleHandle;
#endif