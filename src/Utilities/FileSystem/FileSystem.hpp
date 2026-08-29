#pragma once

namespace N::U::FileSystem {
/** @brief Reads and returns the contents of a file. */
std::string ReadFile(const std::string& path);

/** @brief Writes content to a file. */
void WriteFile(const std::string& path, const std::string& content);
} // namespace N::U::FileSystem
