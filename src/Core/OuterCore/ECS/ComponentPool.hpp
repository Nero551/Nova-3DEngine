#pragma once
#include "Component.hpp"
#include "IComponentPool.hpp"

namespace N {
template <typename T>
concept ComponentType = std::derived_from<T, Component>;

/** @warning CURRENTLY DOESN'T WORK , UNDER CONSTRUCTION
 *
 * GOAL: make components store the entity ids instead of each entity storing its components.
 * this allows systems to loop over their wanted components instead of over ALL entities
 */
template <ComponentType T> struct ComponentPool : IComponentPool {
    T& Add(unsigned int entityId) {
        Indices[entityId] = Components.size();
        EntityIds.emplace_back(entityId);
        Components.emplace_back();

        return Get(entityId);
    }

    T& Get(const unsigned int entityId) {
        return Components[Indices.at(entityId)];
    }

private:
    std::vector<T> Components{};
    std::vector<unsigned int> EntityIds{};
    std::unordered_map<unsigned int, size_t> Indices{};
};
} // namespace N
