#include "XGControl.h"

bool XGControl::keepServerRunning = false;
time_t XGControl::boot_time = time(0);
time_t XGControl::last_query_time = time(0) - DAYS(1); // A day before to prevent it from "waking up" at boot
