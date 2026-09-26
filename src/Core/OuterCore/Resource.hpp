#pragma once
#include "Utilities/DataStructures/GIndexPool.hpp"

namespace N::C
{
/**
 * @brief Base class for resources managed by ResourceManager.
 *
 * Provides a name shared by all resource types and establishes
 * polymorphic destruction through a virtual destructor.
 * Resources are non-copyable but movable.
 */
struct Resource
{
    using Handle = U::GIndexPool<>::Handle;

    Resource(std::string name) : m_Name(std::move(name)) {};

    virtual ~Resource() = default;

    Resource(const Resource&) = delete;

    Resource& operator=(const Resource&) = delete;

    Resource(Resource&&) = default;

    Resource& operator=(Resource&&) = default;

    Handle GetHandle() const
    {
        return m_Handle;
    }

    const std::string& GetName() const
    {
        return m_Name;
    }

  private:
    friend struct ResourceManager;
    Handle m_Handle;
    std::string m_Name;
};
} // namespace N::C
