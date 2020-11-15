#pragma once
#include <string>
#include <ctime>
#include "TimeUnits.h"

/// <summary>
/// Class to house control variables
/// </summary>
class XGControl
{
public:
	static bool keepServerRunning;
	static time_t boot_time;
	static time_t last_query_time;
};

