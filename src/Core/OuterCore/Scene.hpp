#pragma once

#include "ECS/Entity.hpp"
#include "Utilities/CheckedPtr.hpp"

namespace N {
/**
 * @brief Temporary container for a group of entities.
 */
struct Scene {
    /** @brief Root entity of the scene hierarchy. */

    Scene() = default;
    virtual ~Scene() = default;

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    Scene(Scene&&) = default;

    Entity& GetRoot();
    void SetRoot(const Entity& entity);
    void SetRoot(unsigned int entityId);

private:
    unsigned int Root;
};
} // namespace N
