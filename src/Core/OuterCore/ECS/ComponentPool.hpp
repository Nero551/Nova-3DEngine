#pragma once
#include "Component.hpp"
#include "IComponentPool.hpp"

#include "Core/Services/EventBus.hpp"
#include "World/Events/EntityDestroyed.hpp"

namespace N {
template <typename T>
concept ComponentType = std::derived_from<T, Component>;

template <ComponentType T> struct ComponentPool : IComponentPool {
    struct Iterator {
        ComponentPool& Pool;
        size_t Index;

        std::pair<unsigned int, T&> operator*() const {
            return { Pool.EntityIds[Index], Pool.Components[Index] };
        }

        Iterator& operator++() {
            ++Index;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return Index != other.Index;
        }
    };

    Iterator begin() {
        return { *this, 0 };
    }

    Iterator end() {
        return { *this, Components.size() };
    }

    ComponentPool() {
        Service::Get<EventBus>().Sub<EntityDestroyed>([this](const EntityDestroyed& event) {
            if (HasId(event.entity.Id)) {
                RemoveById(event.entity.Id);
            }
        });
    }

    T& Add(unsigned int entityId) {
        EntityIds.push_back(entityId);
        Indices.emplace(entityId, EntityIds.size() - 1);
        Components.emplace_back();

        return Components.back();
    }

    bool HasId(unsigned int entityId) const {
        return Indices.contains(entityId);
    }

    unsigned int GetIdByIndex(size_t index) const {
        return EntityIds.at(index);
    }

    T& GetComponentByIndex(size_t index) {
        return Components.at(index);
    }

    T& GetComponentById(unsigned int entityId) {
        auto it = Indices.find(entityId);

        if (it != Indices.end()) {
            return Components[it->second];
        }
        U::Logger::Fatal("Component not found");
    }

    void RemoveByIndex(size_t index) {
        Components.erase(Components.begin() + index);
        Indices.erase(EntityIds[index]);
        EntityIds.erase(EntityIds.begin() + index);
    }

    void RemoveById(unsigned int entityId) {
        auto it = Indices.find(entityId);

        if (it != Indices.end()) {
            RemoveByIndex(it->second);
        }
    }

    size_t Size() const {
        return EntityIds.size();
    }

private:
    std::vector<T> Components{};
    std::vector<unsigned int> EntityIds{};
    std::unordered_map<unsigned int, size_t> Indices{};
};
} // namespace N
