#pragma once

#include "../OuterCore/Scene.hpp"
#include "Core/OuterCore/ECS/ComponentQuery/ComponentPoolQuery.hpp"
#include "Core/OuterCore/Service.hpp"
#include "Core/Services/EventBus.hpp"
#include "SystemOwner.hpp"
#include "Utilities/Logger.hpp"
#include "World/Events/EntityCreated.hpp"

namespace N {
template <typename T>
concept EntityType = std::derived_from<T, Entity>;

/**
 * @brief Owns and manages the runtime state of the engine world.
 * The World owns all entities. It is responsible for managing the lifecycle of
 * entities, including entity creation and destruction & system execution.
 */
struct World : SystemOwner {
    U::CheckedPtr<Entity> Root{ "World Has No Root Entity" };
    U::CheckedPtr<Entity> ActiveCamera{ "World Has No Active Camera" };
    ComponentPoolQuery Query;
    int MaxLights = 24;

    /** @brief Gets the global World instance. */
    static World& Get();

    /**
     * @brief Removes an entity from the world.
     * @param id ID of the entity to remove.
     * @note removes all children of the entity as well, and fires an
     * EntityDestroyed event for each removed entity.
     */
    void RemoveEntity(unsigned int id);

    /**
     * @brief Creates and registers an entity in the world.
     *
     * The entity is assigned a unique ID, initialized, and registered with
     * the world's entity collection. An EntityCreated event is fired after
     * initialization.
     *
     * @tparam T Entity type to create.
     * @return Reference to the newly created entity.
     */
    template <EntityType T> T& CreateEntity() {
        const unsigned int id = ++currentEntityId;
        auto entity = std::make_unique<T>();
        entity->Id = id;
        entity->Initialize();

        Entities.emplace(id, std::move(entity));
        Service::Get<EventBus>().Fire<EntityCreated>(static_cast<T&>(*Entities.find(id)->second));

        return static_cast<T&>(*Entities.find(id)->second);
    }

    /**
     * @brief Finds an entity by its ID.
     * @param id ID of the entity to find.
     * @return Reference to the requested entity.
     */
    Entity& FindEntity(unsigned int id);

    /**
     * @brief Attempts to find an entity by its ID.
     * @param id ID of the entity to find.
     * @return pointer to the entity, or null if it does not exist.
     */
    U::CheckedPtr<Entity> TryFindEntity(unsigned int id);

private:
    std::unordered_map<unsigned int, std::unique_ptr<Entity>> Entities;
    /** @brief ID assigned to the most recently created entity. */
    unsigned int currentEntityId = 0;

protected:
    friend struct Engine;
    /** @brief Registers the systems owned by the world. */
    void AddSystems() override;

    void Start();

    void Update(double dt);

    void FixedUpdate(double fdt);

    void Stop();

    void BeginFrame(double dt);

    void EndFrame(double dt);

    void Render();
};
} // namespace N
