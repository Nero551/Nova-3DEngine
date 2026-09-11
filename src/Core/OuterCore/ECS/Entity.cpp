#include "Entity.hpp"
#include "Core/InnerCore/Engine.hpp"

namespace N
{
void Entity::DestroyChild(const unsigned int id)
{
    if (!HasChild(id))
    {
        return;
    }

    auto& child = World::Get().FindEntity(id);
    child.Destroy();
}

void Entity::AttachChild(Entity& child)
{
    if (HasChild(child.Id))
    {
        U::Logger::Error("Child Entity already exists: " + std::to_string(child.Id));
        return;
    }

    if (&child == this)
    {
        U::Logger::Error("An entity cannot be its own child.");
        return;
    }

    if (child.IsDescendantOf(Id))
    {
        U::Logger::Error("An entity cannot have its descendant as a child.");
        return;
    }

    if (child.IsAncestorOf(Id))
    {
        U::Logger::Error("An entity cannot have its ancestor as a child.");
        return;
    }

    if (child.HasParent())
    {
        child.ClearParent();
    }

    Children.insert(child.Id);
    child.Parent = Id;
}

void Entity::DetachChild(const unsigned int id)
{
    if (Children.erase(id))
    {
        World::Get().FindEntity(id).Parent = 0;
    }
}

Entity& Entity::GetChild(const unsigned int id)
{
    if (HasChild(id))
    {
        return World::Get().FindEntity(id);
    }

    U::Logger::Fatal(std::format("Entity {} has no child {}", Id, id));
}

U::CheckedPtr<Entity> Entity::TryGetChild(const unsigned int id)
{
    if (HasChild(id))
    {
        return &World::Get().FindEntity(id);
    }

    return nullptr;
}

bool Entity::HasChild(const unsigned int id) const
{
    return Children.contains(id);
}

size_t Entity::ChildCount() const
{
    return Children.size();
}

std::vector<U::CheckedPtr<Entity>> Entity::GetChildren()
{
    std::vector<U::CheckedPtr<Entity>> children;
    children.reserve(Children.size());

    for (const unsigned int id : Children)
    {
        children.emplace_back(&World::Get().FindEntity(id));
    }

    return children;
}

void Entity::DestroyChildren()
{
    while (!Children.empty())
    {
        DestroyChild(*Children.begin());
    }
}

std::vector<U::CheckedPtr<Entity>> Entity::GetDescendants()
{
    std::vector<U::CheckedPtr<Entity>> descendants;
    descendants.reserve(Children.size());

    RecursiveChildren(descendants, *this);

    return descendants;
}

bool Entity::HasDescendant(const unsigned int id) const
{
    return IsAncestorOf(id);
}

bool Entity::IsDescendantOf(const unsigned int entityId) const
{
    unsigned int current = Parent;

    while (current != 0)
    {
        if (current == entityId)
            return true;

        current = World::Get().FindEntity(current).Parent;
    }

    return false;
}

std::vector<U::CheckedPtr<Entity>> Entity::GetAncestors()
{
    std::vector<U::CheckedPtr<Entity>> ancestors;

    unsigned int current = Parent;

    while (current != 0)
    {
        Entity& entity = World::Get().FindEntity(current);
        ancestors.emplace_back(&entity);
        current = entity.Parent;
    }

    return ancestors;
}

bool Entity::IsAncestorOf(const unsigned int entityId) const
{
    for (const unsigned int id : Children)
    {
        if (id == entityId)
            return true;

        const Entity& child = World::Get().FindEntity(id);

        if (child.IsAncestorOf(entityId))
            return true;
    }

    return false;
}

bool Entity::HasAncestor(const unsigned int id)
{
    return IsDescendantOf(id);
}

U::CheckedPtr<Entity> Entity::TryGetParent()
{
    if (Parent == 0)
    {
        return nullptr;
    }
    return &World::Get().FindEntity(Parent);
}

Entity& Entity::GetParent()
{
    return World::Get().FindEntity(Parent);
}

void Entity::SetParent(Entity& parent)
{
    parent.AttachChild(*this);
}

void Entity::ClearParent()
{
    if (HasParent())
    {
        World::Get().FindEntity(Parent).DetachChild(Id);
    }
}

bool Entity::HasParent() const
{
    return Parent != 0;
}

void Entity::Destroy()
{
    World::Get().RemoveEntity(Id);
}

Entity& Entity::GetRoot()
{
    U::CheckedPtr current = this;

    while (current->HasParent())
    {
        current = &World::Get().FindEntity(current->Parent);
    }

    return *current;
}

void Entity::RecursiveChildren(std::vector<U::CheckedPtr<Entity>>& entities, const Entity& entity)
{
    for (const unsigned int id : entity.Children)
    {
        Entity& child = World::Get().FindEntity(id);

        entities.emplace_back(&child);
        RecursiveChildren(entities, child);
    }
}
} // namespace N