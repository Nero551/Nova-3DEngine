#pragma once

namespace N
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
    using ResourceId = unsigned int;

    std::string Name;

    Resource(std::string name) : Name(std::move(name)) {};

    virtual ~Resource() = default;

    Resource(const Resource&) = delete;

    Resource& operator=(const Resource&) = delete;

    Resource(Resource&&) = default;

    Resource& operator=(Resource&&) = default;

    ResourceId GetResourceId() const
    {
        return m_ResourceId;
    }

  private:
    friend struct ResourceManager;
    ResourceId m_ResourceId = 0;
};
} // namespace N
