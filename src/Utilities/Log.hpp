#pragma once

namespace N::U
{
/** @brief Provides formatted console logging and pointer validation. */
struct Log
{
    /** @brief Prints a message to the console. */
    template <typename... Args> static void Print(const Args&... args)
    {
        (..., (std::cout << args));
        std::cout << '\n';
    }

    /** @brief Logs an informational message. */
    template <typename... Args> static void Info(const Args&... args)
    {
        std::cout << m_Green << "ℹ️ [INFO] " << m_Reset;
        (..., (std::cout << args));
        std::cout << '\n';
    }

    /** @brief Logs a warning message. */
    template <typename... Args> static void Warning(const Args&... args)
    {
        std::cout << m_Yellow << "⚠️ [WARNING] " << m_Reset;
        (..., (std::cout << args));
        std::cout << '\n';
    }

    /** @brief Logs an error message. */
    template <typename... Args> static void Error(const Args&... args)
    {
        std::cout << m_Red << "❌ [ERROR] " << m_Reset;
        (..., (std::cout << args));
        std::cout << '\n';
    }

    /** @brief Logs a fatal error and throws an exception. */
    template <typename... Args> [[noreturn]] static void Fatal(const Args&... args)
    {
        std::cout << m_Red << "💀 [FATAL] " << m_Reset;
        (..., (std::cout << args));
        std::cout << '\n';
        throw std::runtime_error("");
    }

    /** @brief Validates a raw pointer and returns the dereferenced object. */
    template <typename T>
    static T& Require(T* rawPtr, const std::string_view nullMessage = "Attempted to deference a null pointer")
    {
        if (!rawPtr)
        {
            Fatal(nullMessage);
        }
        return *rawPtr;
    }

  private:
    /** @brief Resets the console text color. */
    static constexpr auto m_Reset = "\033[0m";

    /** @brief Sets the console text color to red. */
    static constexpr auto m_Red = "\033[31m";

    /** @brief Sets the console text color to green. */
    static constexpr auto m_Green = "\033[32m";

    /** @brief Sets the console text color to yellow. */
    static constexpr auto m_Yellow = "\033[33m";

    /** @brief Sets the console text color to blue. */
    static constexpr auto m_Blue = "\033[34m";
}; // namespace Log
} // namespace N::U
