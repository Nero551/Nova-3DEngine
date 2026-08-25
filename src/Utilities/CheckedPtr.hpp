#pragma once
#include "Utilities/Logger.hpp"

namespace N::U {
/// Non-owning pointer that checks for null dereferences.
/// Does NOT track object lifetime or prevent dangling pointers.
template <typename T> struct CheckedPtr {
    CheckedPtr() = default;

    CheckedPtr(T* objectPtr) {
        ptr = objectPtr;
    }

    CheckedPtr(const std::string& nullMessage) {
        this->nullMessage += nullMessage;
    }

    CheckedPtr(T* objectPtr, const std::string& nullMessage) {
        ptr = objectPtr;
        this->nullMessage += nullMessage;
    }

    CheckedPtr& operator=(T* objectPtr) {
        ptr = objectPtr;
        return *this;
    }

    T* operator->() const {
        return &Logger::Require(ptr, nullMessage);
    }

    bool operator==(T* otherPtr) {
        return ptr == otherPtr;
    }

    T& operator*() const {
        return Logger::Require(ptr, nullMessage);
    }

    explicit operator bool() const {
        return ptr != nullptr;
    }

    void Reset() {
        ptr = nullptr;
    }

    [[nodiscard]] T* Get() const {
        return ptr;
    }

    [[nodiscard]] bool IsNull() const {
        return ptr == nullptr;
    }

private:
    T* ptr = nullptr;
    std::string nullMessage = "[NULL PTR] ";
};
} // namespace N::U
