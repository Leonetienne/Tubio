#pragma once
#include "XGControl.h"

#ifdef _WIN
#include <Windows.h>
#else
#include <sys/wait.h>
#include <unistd.h>
#endif

class Idler
{
public:
	static void Update();
};
