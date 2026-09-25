#include "Scene.hpp"

#include "Core/InnerCore/World.hpp"

N::Entity& N::Scene::GetRoot() const
{
    return World::Get().FindEntity(m_Root);
}

void N::Scene::SetRoot(const Entity& entity)
{
    m_Root = entity.GetId();
}

void N::Scene::SetRoot(const unsigned int entityId)
{
    m_Root = entityId;
}
