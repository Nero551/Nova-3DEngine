#pragma once

#include "Utilities/CheckedPtr.hpp"
#include "Utilities/Logger.hpp"

namespace N {
/**

* @brief Represents an entity in the world.
*
* An entity is a container for components identified by a unique ID.
* Entities can also form hierarchical parent-child relationships.
*
* Components are stored by their type and each entity may contain at most
* one component of a given type.
  */
struct Entity {
    unsigned int Id = 0;

    virtual ~Entity() = default;

    Entity() = default;

    Entity(const Entity&) = delete;

    Entity& operator=(const Entity&) = delete;

    Entity(Entity&&) = default;

    Entity& operator=(Entity&&) = default;

    /**

    * @brief Initializes the entity.
    *
    * Intended to be overridden by derived entity types that require
    * initialization.
      */
    virtual void Initialize() {
    }
    /**

    * @brief Destroys a direct child entity.
    *
    * @param id Id of the child entity to destroy.
      */
    void DestroyChild(unsigned int id);

    /**

    * @brief Attaches an entity as a direct child.
    * The child is detached from its previous parent if it already has one.
    * Cyclic parent-child relationships are rejected.
    *
    * @param child Entity to attach.
      */
    void AttachChild(Entity& child);

    /**
     * @brief Detaches a direct child from this entity.
     * @param id Id of the child entity to detach.
     */
    void DetachChild(unsigned int id);

    /**

    * @brief Gets a direct child by ID.
    * Logs a fatal error if the child does not exist.
    *
    * @param id Id of the child entity.
    * @return Reference to the requested child.
      */
    Entity& GetChild(unsigned int id);

    /**
     * @brief Attempts to get a direct child by ID.
     *
     * @param id Id of the child entity.
     * @return pointer to the child, or null if it does not exist.
     */
    U::CheckedPtr<Entity> TryGetChild(unsigned int id);

    /**
     * @brief Checks whether this entity has a direct child with the given Id.
     * @param id Id of the child entity.
     * @return True if the child exists.
     */
    bool HasChild(unsigned int id) const;

    /**
     * @brief Gets the number of direct children.
     * @return Number of direct children.
     */
    size_t ChildCount() const;

    /**
     * @brief Gets all direct children of the entity.
     * @return A vector containing pointers to the children.
     */
    std::vector<U::CheckedPtr<Entity>> GetChildren();

    /** @brief Destroys all direct children of the entity. */
    void DestroyChildren();

    /**
     * @brief Gets all descendants of the entity.
     * Descendants include children, grandchildren, and all deeper entities
     * in the hierarchy.
     * @return A vector containing pointers to all descendants.
     */
    std::vector<U::CheckedPtr<Entity>> GetDescendants();

    /**
     * @brief Checks whether an entity is a descendant of this entity.
     * @param id Id of the entity to check.
     * @return True if the entity is a descendant.
     */
    bool HasDescendant(unsigned int id) const;

    /**
     * @brief Checks whether this entity is a descendant of another entity.
     * @param entity Potential ancestor entity.
     * @return True if this entity is a descendant of the given entity.
     */
    bool IsDescendantOf(const Entity& entity);

    /**
     * @brief Gets all ancestors of the entity.
     * The returned list contains the parent, grandparent, and all higher
     * ancestors in the hierarchy.
     * @return A vector containing pointers to all ancestors.
     */
    std::vector<U::CheckedPtr<Entity>> GetAncestors();

    /**
     * @brief Checks whether an entity is an ancestor of this entity.
     * @param entity Potential descendant entity.
     * @return True if this entity is an ancestor.
     */
    bool IsAncestorOf(const Entity& entity);

    /**
     * @brief Checks whether this entity has the specified ancestor.
     * @param id Id of the potential ancestor.
     * @return True if the entity is an ancestor.
     */
    bool HasAncestor(unsigned int id) const;

    /**
     * @brief Gets the entity's parent.
     * @return Reference to the parent entity.
     */
    Entity& GetParent();

    /**
     * @brief Sets the parent of this entity.
     * @param parent new parent entity.
     */
    void SetParent(Entity& parent);

    /** @brief Removes the entity from its current parent. */
    void ClearParent();

    /**
     * @brief Checks whether the entity has a parent.
     * @return True if the entity has a parent.
     */
    bool HasParent() const;

    /** @brief Destroys this entity. */
    void Destroy();

    /**
     * @brief Gets the root entity of this hierarchy.
     * Traverses the parent chain until an entity without a parent is found.
     * @return Reference to the root entity.
     */
    Entity& GetRoot();

private:
    std::vector<unsigned int> Children;

    /** @brief Parent entity in the hierarchy. */
    unsigned int Parent{};

    /**
     * @brief Recursively collects all descendants of an entity.
     * @param entities Vector to append discovered descendants to.
     * @param entity Entity whose descendants should be traversed.
     */
    void RecursiveChildren(std::vector<U::CheckedPtr<Entity>>& entities, const Entity& entity);
};
} // namespace N
