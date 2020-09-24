#include "Filesystem.h"

std::string FileSystem::ReadFile(std::string filename)
{
    std::ifstream ifs;
    ifs.open(filename, std::ios::in);
    if (!ifs.good())
    {
        throw std::exception("no such file");
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
