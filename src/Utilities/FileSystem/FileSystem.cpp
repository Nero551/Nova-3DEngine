#include "FileSystem.hpp"
#include "Utilities/Log.hpp"

namespace N::U
{
std::string FileSystem::ReadFile(const std::string& path)
{
    std::ifstream file(path);

    if (!file)
    {
        Log::Error("Failed to read file: ", path);
        return {};
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

void FileSystem::WriteFile(const std::string& path, const std::string& content)
{
    std::ofstream file(path);

    if (!file)
    {
        Log::Error("Failed to write file: ", path);
        return;
    }

    file << content;
}
} // namespace N::U
