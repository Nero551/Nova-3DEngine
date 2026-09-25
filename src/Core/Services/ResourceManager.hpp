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
        Resource::Handle handle = m_Handles.Acquire();
        resource->m_Handle = handle;
        resource->m_Name = name;
        m_ResourceLookup.emplace(std::move(key), handle.Index);
        return static_cast<T&>(*m_Resources.Emplace(handle.Index, std::move(resource))->Value);
    }

    void Unload(const Resource::Handle& handle)
    {
        if (!m_Resources.Contains(handle.Index) || !m_Handles.IsAcquired(handle))
            return;

        Resource& resource = *m_Resources[handle.Index];

        m_ResourceLookup.erase(typeid(resource).name() + resource.m_Name);

        m_Resources.Erase(handle.Index);
        m_Handles.Release(handle);
    }

    template <typename T> T& Acquire(const Resource::Handle& handle) const
    {
        if (!m_Handles.IsAcquired(handle))
            U::Log::Fatal(
                "Resource Handle: [", handle.Index, " | ", handle.Generation, "]", " Doesn't Exist.");

        return static_cast<T&>(*m_Resources[handle.Index]);
    }

    template <ResourceType T> T& Acquire(const std::string& name)
    {
        const auto it = m_ResourceLookup.find(typeid(T).name() + name);

        if (it == m_ResourceLookup.end())
        {
            U::Log::Fatal("Resource Doesn't Exist: ", name);
        }

        return static_cast<T&>(*m_Resources[it->second]);
    }
    template <ResourceType T> bool Exists(const std::string& name) const
    {
        return m_ResourceLookup.contains(typeid(T).name() + name);
    }

    bool Exists(const Resource::Handle& handle) const
    {
        return m_Resources.Contains(handle.Index) && m_Handles.IsAcquired(handle);
    }

  private:
    U::SparseSet<std::unique_ptr<Resource>> m_Resources{};
    std::unordered_map<std::string, unsigned int> m_ResourceLookup{};
    U::GIndexPool<> m_Handles{};
};
} // namespace N
