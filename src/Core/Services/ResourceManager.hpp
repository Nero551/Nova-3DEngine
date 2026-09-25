#pragma once

#include "Core/OuterCore/Resource.hpp"
#include "Core/OuterCore/Service.hpp"
#include "Utilities/DataStructures/IndexPool.hpp"
#include "Utilities/DataStructures/SparseSet.hpp"

namespace N
{
template <typename T>

/** @brief Concept for all structs inheriting Resource , represents all objects loadable by ResourceManager */
concept ResourceType = std::derived_from<T, Resource>;

/** @brief Manages the lifetime and retrieval of resources. */
struct ResourceManager : Service
{
    /**
     * @brief Loads a resource or returns the already loaded instance.
     * If a resource with the given name already exists, its existing instance
     * is returned. Otherwise, a new instance is constructed and stored.
     * @tparam T Resource type to load.
     * @param name Unique name used to identify the resource.
     * @param args Arguments passed to T's constructor after the resource name.
     * @return Reference to the loaded resource.
     */
    template <ResourceType T, typename... Args>
    T& Load(const std::string& name, Args&&... args)
        requires std::constructible_from<T, const std::string&, Args...>
    {
        std::string key = typeid(T).name() + name;
        if (const auto it = m_ResourceLookup.find(key); it != m_ResourceLookup.end())
        {
            // N::U::Logger::Warning("Resource: " + name + " Already Loaded.");
            return static_cast<T&>(*m_Resources[it->second]);
        }

        auto resource = std::make_unique<T>(name, std::forward<Args>(args)...);
        Resource::ResourceId id = m_AvailableIds.Acquire();
        resource->m_ResourceId = id;
        m_ResourceLookup.emplace(std::move(key), id);
        return static_cast<T&>(*m_Resources.Emplace(id, std::move(resource))->Value);
    }

    void Unload(const Resource::ResourceId id)
    {
        if (m_Resources.Contains(id))
        {
            m_Resources.Erase(id);
            m_AvailableIds.Release(id);
        }
    }

    template <typename T> T& Acquire(const Resource::ResourceId id) const
    {
        return static_cast<T&>(*m_Resources[id]);
    }

    template <typename T> T& Acquire(const std::string& name)
    {
        return static_cast<T&>(*m_Resources[m_ResourceLookup.at(typeid(T).name() + name)]);
    }

    template <ResourceType T> bool Exists(const std::string& name) const
    {
        return m_ResourceLookup.contains(typeid(T).name() + name);
    }

    bool Exists(const Resource::ResourceId id) const
    {
        return m_Resources.Contains(id);
    }

  private:
    U::SparseSet<std::unique_ptr<Resource>> m_Resources{};
    std::unordered_map<std::string, Resource::ResourceId> m_ResourceLookup{};
    U::IndexPool<unsigned int> m_AvailableIds{};
};
} // namespace N
