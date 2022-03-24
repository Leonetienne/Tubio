#include "FileSystem.h"
#ifdef _WIN
#include <Windows.h>
#endif

std::string FileSystem::ReadFile(std::string filename)
{
    std::ifstream ifs;
    ifs.open(filename, std::ios::in);
    if (!ifs.good())
    {
        throw std::exception();
        std::terminate();
    }
    std::string buf;
    std::stringstream content;
    while (std::getline(ifs, buf))
    {
        content << buf;
    }
    return content.str();
}

bool FileSystem::WriteFile(std::string filename, std::string content)
{
    std::ofstream ofs;
    if (!ofs.good()) return false;
    ofs.open(filename, std::ios::out);
    ofs << content;
    ofs.close();
    return true;
}

bool FileSystem::Exists(std::string filename)
{
    std::ifstream ifs;
    ifs.open(filename, std::ios::in);
    if (!ifs.good()) return false;
    ifs.close();
    return true;
}

bool FileSystem::ExistsDirectory(std::string name)
{
    return (!Exists(name) && (std::filesystem::exists(name)));
}

bool FileSystem::_CreateDirectory(std::string name)
{
    return std::filesystem::create_directories(name);
}

bool FileSystem::CreateDirectoryIfNotExists(std::string name)
{
    if (!ExistsDirectory(name)) return _CreateDirectory(name);
    return false;
}

bool FileSystem::DeleteDirectory(std::string name)
{
    return std::filesystem::remove_all(name);
}

bool FileSystem::Copy(std::string from, std::string to)
{
    std::ifstream ifs;
    if (!ifs.good()) return false;
    ifs.open(from, std::ios::in | std::ios::binary);
    
    std::ofstream ofs;
    if (!ofs.good()) return false;
    ofs.open(to, std::ios::out | std::ios::binary);

    std::copy(std::istreambuf_iterator<char>(ifs),
        std::istreambuf_iterator<char>(),
        std::ostreambuf_iterator<char>(ofs));

    return true;
}

bool FileSystem::Delete(std::string filename)
{
    if (!Exists(filename)) return false;
    remove(filename.c_str());
    return true;
}

#include <iostream>
long long int FileSystem::CalculateSize(std::string name, bool isDirectory)
{
#ifdef _WIN
    // Windows implementation

    WIN32_FIND_DATAA data;
    HANDLE sh = NULL;
    long long int byteCount = 0;
    
    //std::cout << "Scanning dir " << name << std::endl;
    sh = FindFirstFileA((name + ((isDirectory) ? "\\*" : "")).c_str(), &data);

    if (sh == INVALID_HANDLE_VALUE) return -1;

    do
    {
        if ((std::string(data.cFileName).compare(".") != 0) && (std::string(data.cFileName).compare("..") != 0))
        {
            // If found file-object is...
            if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
            {
                // ... a directory, search it recursively
                byteCount += CalculateSize(name + "\\" + data.cFileName, true);
            }
            else
            {
                // ... a file, get its size
                byteCount += (long long int)((data.nFileSizeHigh * MAXDWORD) + data.nFileSizeLow);
            }
        }

    } while (FindNextFileA(sh, &data));

    FindClose(sh);
    return byteCount;

#else
    // Linux implementation

    std::string cmd("du -sb '");
    cmd.append(name);
    cmd.append("' | cut -f1 2>&1");

    // execute above command and get the output
    FILE* stream = popen(cmd.c_str(), "r");
    if (stream) {
        const int max_size = 256;
        char readbuf[max_size];
        if (fgets(readbuf, max_size, stream) != NULL) {
            return atoll(readbuf);
        }
        pclose(stream);
    }
    // return error val
    return -1;
#endif
}
