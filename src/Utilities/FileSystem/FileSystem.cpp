#include "FileSystem.hpp"

#include <fstream>
#include <sstream>

#include "Utilities/Logger.hpp"

namespace N::U {
std::string FileSystem::ReadFile(const std::string& path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();

    if (buffer.str().empty()) {
        Logger::Error("File Doesn't Exist: " + path);
    }

    return buffer.str();
}

void FileSystem::WriteFile(const std::string& path, const std::string& content) {
    std::ofstream file(path);

    if (!file) {
        Logger::Error("Failed to write file: " + path);
        return;
    }

    file << content;
}
} // namespace N::U
