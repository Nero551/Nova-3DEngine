#pragma once
#include "../ComponentPool.hpp"
#include "../IComponentPool.hpp"
#include "ComponentPoolQueryResult.hpp"

namespace N {
struct ComponentPoolQuery {
    template <ComponentType T> ComponentPool<T>& Pool() {
        const auto type = std::type_index(typeid(T));
        auto it = ComponentPools.find(type);

        if (it == ComponentPools.end()) {
            it = ComponentPools.emplace(type, std::make_unique<ComponentPool<T>>()).first;
        }
        return static_cast<ComponentPool<T>&>(*it->second);
    }

    template <ComponentType... Args> N::ComponentPoolQueryResult<Args...> With() {
        auto Pools = GetPools<Args...>();
        auto& firstPool = std::get<0>(Pools);
        N::ComponentPoolQueryResult<Args...> result;

        std::apply(
            [&](auto&... pools) {
                for (size_t i = 0; i < firstPool.Size(); ++i) {
                    const unsigned int id = firstPool.GetIdByIndex(i);

                    if ((pools.HasId(id) && ...)) {
                        result.EntityIds.push_back(id);

                        AddComponents(result, id, Pools, std::index_sequence_for<Args...>{});
                    }
                }
            },
            Pools);

        return result;
    }

private:
    std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> ComponentPools{};

    template <ComponentType... Args, size_t... I> void AddComponents(N::ComponentPoolQueryResult<Args...>& result,
        unsigned int id, std::tuple<ComponentPool<Args>&...>& pools, std::index_sequence<I...>) {
        (std::get<I>(result.Components).push_back(&std::get<I>(pools).GetComponentById(id)), ...);
    }

    template <ComponentType... Args> std::tuple<ComponentPool<Args>&

        ...>
    GetPools() {
        return std::tuple<ComponentPool<Args>&...>{ Pool<Args>()... };
    }
};
} // namespace N
