#include "Scene.hpp"

#include "Core/InnerCore/World.hpp"

N::Entity& N::Scene::GetRoot() {
    return World::Get().FindEntity(Root);
}

void N::Scene::SetRoot(const Entity& entity) {
    Root = entity.Id;
}

void N::Scene::SetRoot(const unsigned int entityId) {
    Root = entityId;
}
