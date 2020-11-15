#pragma once

/* These are APPROXIMATIONS */
/* UNITS ARE SECONDS */
#define SECONDS(x) (x)
#define MINUTES(x) (SECONDS(60) * x)
#define HOURS(x) (MINUTES(60) * x)
#define DAYS(x) (HOURS(24) * x)
#define WEEKS(x) (DAYS(7) * x)
#define MONTHS(x) (WEEKS(4) * x)
#define YEARS(x) (MONTHS(12) * x)
