#pragma once
#include "Core/OuterCore/ECS/System.hpp"
#include "Utilities/Logger.hpp"

namespace N {
/** @brief Restricts types to classes derived from System. */
template <typename T>
concept SystemType = std::derived_from<T, System>;

/**
 * @brief Owns and manages a collection of ECS systems.
 * @details Used as a base class by Module and World to provide system
 * registration and type-safe system access.
 */
struct SystemOwner {
    /** @brief Provides a virtual destructor for derived system owners. */
    virtual ~SystemOwner() = default;

    /** @brief Returns a registered system of the specified type. */
    template <SystemType T> T& GetSystem() {
        auto system = Systems.find(typeid(T));
        if (system == Systems.end()) {
            U::Logger::Fatal(std::format("System Not Found: {}", typeid(T).name()));
        }
        return static_cast<T&>(*system->second);
    }

protected:
    /** @brief Stores systems indexed by their runtime type. */
    std::unordered_map<std::type_index, std::unique_ptr<System>> Systems;

    /** @brief Adds the systems owned by the derived class. */

    /** @brief Creates and registers a system of the specified type. */
    template <SystemType T> T& AddSystem() {
        if (Systems.contains(std::type_index(typeid(T)))) {
            U::Logger::Fatal("System Already Exists");
        }

        auto system = std::make_unique<T>();
        T& ref = *system;
        Systems.emplace(typeid(T), std::move(system));
        return ref;
    }
};
} // namespace N
