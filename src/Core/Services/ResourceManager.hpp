#pragma once

#include "Core/OuterCore/Resource.hpp"
#include "Core/OuterCore/Service.hpp"
#include "Utilities/Logger.hpp"

namespace N
{
template <typename T>

/** @brief Concept for all structs inheriting Resource , represents all objects loadable
   by ResourceManager */
concept ResourceType = std::derived_from<T, Resource>;

/**
 * @brief Manages the lifetime and retrieval of resources.
 */
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
    template <ResourceType T, typename... Args> T& Load(const std::string& name, Args&&... args)
    {
        std::string key = typeid(T).name() + name;
        if (Resources.contains(key))
        {
            N::U::Logger::Warning("Resource: " + name + " Already Loaded.");
            return static_cast<T&>(*Resources.at(key));
        }

        if constexpr (!std::constructible_from<T, const std::string&, Args...>)
        {
            U::Logger::Fatal("Resource: " + name + " ,Of Type: " + typeid(T).name() +
                " Can't Be Constructed From the Given Arguments.");
        }
        else
        {
            auto resource = std::make_unique<T>(name, std::forward<Args>(args)...);
            Resources.emplace(key, std::move(resource));

            return static_cast<T&>(*Resources.at(key));
        }
    }

    template <ResourceType T> bool Exists(const std::string& name)
    {
        std::string key = typeid(T).name() + name;
        if (Resources.contains(key))
        {
            return true;
        }
        return false;
    }

  private:
    std::unordered_map<std::string, std::unique_ptr<Resource>> Resources;
};
} // namespace N
