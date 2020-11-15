#include "Idler.h"
#include <iostream>
#define TIME_TO_FALL_ASLEEP MINUTES(5)
#define SLEEP_TIME SECONDS(3)

void Idler::Update()
{
	// Has no request been made within 3 minutes?
	if (time(0) - XGControl::last_query_time > TIME_TO_FALL_ASLEEP)
	{
		// Let the processor chill for a second.
		// This should reduce the idling-cpu load to near 0%
#ifdef _WIN
		std::cout << "Sleeping..." << std::endl;
		Sleep(SLEEP_TIME * 1000); // Uses milliseconds
		std::cout << "Waking..." << std::endl;
#else
		usleep(SLEEP_TIME * 10000000); // Uses microseconds
#endif
	}
}
