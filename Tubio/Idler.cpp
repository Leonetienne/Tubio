#include "Idler.h"
#include <iostream>
#define TIME_TO_FALL_ASLEEP MINUTES(3)
#define SLEEP_TIME SECONDS(3)

void Idler::Update()
{
	// Has no request been made within x minutes?
	if (time(0) - XGControl::last_query_time > TIME_TO_FALL_ASLEEP)
	{
		// Let the processor chill for a second.
		// This should reduce the idling-cpu load to near 0%
#ifdef _WIN
		Sleep((DWORD)(SLEEP_TIME * 1e3)); // Uses milliseconds
#else
		usleep(SLEEP_TIME * 1e6); // Uses microseconds
#endif
	}
}
