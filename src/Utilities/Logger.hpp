#pragma once

namespace N::U {
/** @brief Provides formatted console logging and pointer validation. */
struct Logger {
    /** @brief Prints a message to the console. */
    template <typename T> static void Print(const T& message) {
        std::cout << message << "\n";
    }

    /** @brief Logs an informational message. */
    template <typename... Args> static void Info(const Args&... args) {
        std::cout << Green << "ℹ️ [INFO] " << Reset;
        (..., (std::cout << args));
        std::cout << '\n';
    }

    /** @brief Logs a warning message. */
    template <typename... Args> static void Warning(const Args&... args) {
        std::cout << Yellow << "⚠️ [WARNING] " << Reset;
        (..., (std::cout << args));
        std::cout << '\n';
    }

    /** @brief Logs an error message. */
    template <typename... Args> static void Error(const Args&... args) {
        std::cout << Red << "❌ [ERROR] " << Reset;
        (..., (std::cout << args));
        std::cout << '\n';
    }

    /** @brief Logs a fatal error and throws an exception. */
    template <typename... Args> [[noreturn]] static void Fatal(const Args&... args) {
        std::cout << Red << "💀 [FATAL] " << Reset;
        (..., (std::cout << args));
        std::cout << '\n';
        throw std::runtime_error("");
    }

    /** @brief Validates a raw pointer and returns the referenced object. */
    template <typename T> static T& Require(T* rawPtr, const std::string& message) {
        if (!rawPtr) {
            Fatal(message);
        }
        return *rawPtr;
    }

private:
    /** @brief Resets the console text color. */
    static constexpr auto Reset = "\033[0m";

    /** @brief Sets the console text color to red. */
    static constexpr auto Red = "\033[31m";

    /** @brief Sets the console text color to green. */
    static constexpr auto Green = "\033[32m";

    /** @brief Sets the console text color to yellow. */
    static constexpr auto Yellow = "\033[33m";

    /** @brief Sets the console text color to blue. */
    static constexpr auto Blue = "\033[34m";
};
} // namespace N::U
