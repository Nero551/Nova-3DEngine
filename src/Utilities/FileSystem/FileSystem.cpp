#include "FileSystem.hpp"

#include <fstream>
#include <sstream>

#include "Utilities/Log.hpp"

namespace N::U
{
std::string FileSystem::ReadFile(const std::string& path)
{
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();

    if (buffer.str().empty())
    {
        Log::Error("File Doesn't Exist: " + path);
    }

    return buffer.str();
}

void FileSystem::WriteFile(const std::string& path, const std::string& content)
{
    std::ofstream file(path);

    if (!file)
    {
        Log::Error("Failed to write file: " + path);
        return;
    }

    file << content;
}
} // namespace N::U
