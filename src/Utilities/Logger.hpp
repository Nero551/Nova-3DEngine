#pragma once

namespace N::U {
struct Logger {
    template <typename T> static void Print(const T& message) {
        std::cout << message << "\n";
    }

    template <typename... Args> static void Info(const Args&... args) {
        std::cout << Green << "[INFO] " << Reset;
        (..., (std::cout << args));
        std::cout << '\n';
    }

    template <typename... Args> static void Warning(const Args&... args) {
        std::cout << Yellow << "[WARNING] " << Reset;
        (..., (std::cout << args));
        std::cout << "\n";
    }

    template <typename... Args> static void Error(const Args&... args) {
        std::cout << Red << "[ERROR] " << Reset;
        (..., (std::cout << args));
        std::cout << '\n';
    }

    template <typename... Args> [[noreturn]] static void Fatal(const Args&... args) {
        std::cout << Red << "[FATAL] " << Reset;
        (..., (std::cout << args));
        std::cout << '\n';
        throw std::runtime_error("");
    }

    template <typename T> static T& Require(T* rawPtr, const std::string& message) {
        if (!rawPtr) {
            Fatal(message);
        }
        return *rawPtr;
    }

private:
    static constexpr auto Reset = "\033[0m";
    static constexpr auto Red = "\033[31m";
    static constexpr auto Green = "\033[32m";
    static constexpr auto Yellow = "\033[33m";
    static constexpr auto Blue = "\033[34m";
};
} // namespace N::U
