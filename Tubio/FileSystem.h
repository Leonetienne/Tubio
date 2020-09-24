#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>

class FileSystem

{
public:
	static std::string ReadFile(std::string filename);
	static bool WriteFile(std::string filename, std::string content);
	static bool Exists(std::string filename);
	static bool Copy(std::string from, std::string to);
	static bool Delete(std::string filename);

private:

};

