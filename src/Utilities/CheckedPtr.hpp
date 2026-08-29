#pragma once
#include "Utilities/Logger.hpp"

namespace N::U {
/**
 * @brief Non-owning pointer wrapper that checks for null dereferences.
 * @note Does not track object lifetime or prevent dangling pointers.
 */
template <typename T> struct CheckedPtr {
    /** @brief Constructs a null CheckedPtr. */
    CheckedPtr() = default;

    /** @brief Constructs a CheckedPtr from a raw pointer. */
    CheckedPtr(T* objectPtr) {
        ptr = objectPtr;
    }

    /** @brief Constructs a null CheckedPtr with a custom error message. */
    CheckedPtr(const std::string& nullMessage) {
        this->nullMessage += nullMessage;
    }

    /** @brief Constructs a CheckedPtr from a pointer with a custom error message. */
    CheckedPtr(T* objectPtr, const std::string& nullMessage) {
        ptr = objectPtr;
        this->nullMessage += nullMessage;
    }

    /** @brief Assigns a raw pointer to the CheckedPtr. */
    CheckedPtr& operator=(T* objectPtr) {
        ptr = objectPtr;
        return *this;
    }

    /** @brief Accesses a member through the pointer, checking for null. */
    T* operator->() const {
        return &Logger::Require(ptr, nullMessage);
    }

    /** @brief Compares the stored pointer with a raw pointer. */
    bool operator==(T* otherPtr) const {
        return ptr == otherPtr;
    }

    /** @brief Compares the stored pointer with another checked pointer. */
    bool operator==(const CheckedPtr<T>& otherCheckedPtr) const {
        return ptr == otherCheckedPtr.ptr;
    }

    /** @brief Dereferences the pointer, checking for null. */
    T& operator*() const {
        return Logger::Require(ptr, nullMessage);
    }

    /** @brief Checks whether the pointer is non-null. */
    explicit operator bool() const {
        return ptr != nullptr;
    }

    /** @brief Resets the pointer to null. */
    void Reset() {
        ptr = nullptr;
    }

    /** @brief Returns the stored raw pointer without validation. */
    [[nodiscard]] T* Get() const {
        return ptr;
    }

    /** @brief Checks whether the pointer is null. */
    [[nodiscard]] bool IsNull() const {
        return ptr == nullptr;
    }

private:
    T* ptr = nullptr;
    std::string nullMessage = "[NULL PTR] ";
};
} // namespace N::U
