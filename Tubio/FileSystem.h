#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <filesystem>

class FileSystem

{
public:
	static std::string ReadFile(std::string filename);
	static bool WriteFile(std::string filename, std::string content);
	static bool Exists(std::string filename);
	static bool Copy(std::string from, std::string to);
	static bool Delete(std::string filename);
	static bool ExistsDirectory(std::string name);
	static bool _CreateDirectory(std::string name);
	static bool CreateDirectoryIfNotExists(std::string name);
	static bool DeleteDirectory(std::string name);
	static long long int CalculateSize(std::string name, bool isDirectory = false);

private:

};

