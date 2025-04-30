#ifndef __ECS_MANAGER__
#define __ECS_MANAGER__ 1

#include <vector>
#include <optional>
#include <functional>
#include <unordered_map>
#include <memory>
#include <iostream>
#include "ModelLoader/Model.hpp"

using Entity = unsigned int;

/**
 * @class ComponentListBase
 * @brief Abstract base class for component containers.
 *
 * This class serves as a base for type-specific component lists,
 * allowing the ECS manager to store and manage different component types
 * in a type-agnostic way.
 */
struct ComponentListBase {
  /**
   * @brief Virtual destructor for proper cleanup.
   */
  virtual ~ComponentListBase() {};

  /**
   * @brief Grows the component list to accommodate a specified number of entities.
   *
   * @param current_entities The number of entities to ensure capacity for.
   */
  virtual void grow(unsigned int current_entities) = 0;
  virtual void resetComponent(Entity entity) = 0;
};

/**
 * @class ComponentList
 * @brief Type-specific container for components of a particular type.
 *
 * Stores components of type T in a vector, with optional values to handle
 * the sparse nature of entity-component relationships.
 *
 * @tparam T The component type stored in this list.
 */
template <typename T>
struct ComponentList : ComponentListBase {
  /**
   * @brief Vector of optional components, indexed by entity ID.
   *
   * If an entity has this component type, the corresponding entry in this vector
   * will contain a value. Otherwise, it will be nullopt.
   */
  std::vector<std::optional<T>> componentlist;

  /**
   * @brief Grows the component list to accommodate a specified number of entities.
   *
   * @param current_entities The number of entities to ensure capacity for.
   */
  virtual void grow(unsigned int current_entities) override;
  virtual void resetComponent(Entity entity) override {
    if (entity < componentlist.size()) {
      componentlist[entity] = std::nullopt;
    }
  }
};


/**
 * @class ECSManager
 * @brief Manager class for the Entity-Component-System architecture.
 *
 * The ECSManager is responsible for creating and managing entities and components.
 * It maintains the relationship between entities and their components, and provides
 * methods for adding, retrieving, and modifying components.
 */
class ECSManager {
public:
  /**
   * @brief Constant representing an invalid entity ID.
   */
  const Entity INVALID_Entity = 0;

  /**
   * @brief Creates a new entity.
   *
   * @return The ID of the newly created entity.
   */
  Entity createEntity();

  /**
   * @brief Destroys an entity and its components.
   *
   * @param entity The ID of the entity to destroy.
   */
  void destroyEntity(Entity entity);

  /**
   * @brief Checks if an entity is still alive.
   *
   * @param entity The ID of the entity to check.
   * @return true if the entity is alive, false otherwise.
   */
  bool isEntityAlive(Entity entity) const;

  Entity get_nextEntity();

  /**
   * @brief Registers a component type with the ECS manager.
   *
   * This method must be called for each component type before entities can have
   * components of that type.
   *
   * @tparam T The component type to register.
   */
  template<typename T>
  void addComponentType();

  /**
   * @brief Gets a component of the specified type for an entity.
   *
   * @tparam T The component type to retrieve.
   * @param entity The ID of the entity to get the component for.
   * @return A pointer to the component if it exists, nullopt otherwise.
   */
  template<typename T>
  std::optional<T*> getComponent(size_t entity);

  /**
   * @brief Adds a component of the specified type to an entity.
   *
   * @tparam T The component type to add.
   * @param entity The ID of the entity to add the component to.
   * @return 0 on success, -1 on failure.
   */
  template<typename T>
  int addComponent(size_t entity);

  /**
   * @brief Edits a component of the specified type for an entity.
   *
   * @tparam T The component type to edit.
   * @param entity The ID of the entity to edit the component for.
   * @param editor A function that modifies the component.
   * @return true if the component was edited successfully, false otherwise.
   */
  template<typename T>
  bool editComponent(size_t entity, const std::function<void(T&)>& editor);

  std::vector<std::shared_ptr<Model>> resources;

private:
  /**
   * @typedef map_type
   * @brief Type definition for the map of component lists.
   *
   * Maps component type hash codes to their respective component lists.
   */
  typedef std::unordered_map<size_t, std::unique_ptr<ComponentListBase>> map_type;

  /**
   * @brief Map of component lists, indexed by component type hash.
   */
  map_type componentListMap_;

  /**
   * @brief The next entity ID to be assigned.
   */
  Entity nextEntity = 1;

  /**
   * @brief List of entity IDs that have been destroyed.
   *
   * These IDs can be reused when creating new entities.
   */
  std::vector<Entity> deadEntities_;
};



/**
 * @brief Registers a component type with the ECS manager.
 *
 * Creates a new component list for the specified type and adds it to the manager.
 *
 * @tparam T The component type to register.
 */

template<typename T>
void ECSManager::addComponentType() {
  size_t key = typeid(T).hash_code();
  std::unique_ptr<ComponentList<T>> value = std::make_unique<ComponentList<T>>();
  componentListMap_.emplace(key, std::move(value));
}

/**
 * @brief Gets a component of the specified type for an entity.
 *
 * @tparam T The component type to retrieve.
 * @param entity The ID of the entity to get the component for.
 * @return A pointer to the component if it exists, nullopt otherwise.
 */
template<typename T>
std::optional<T*> ECSManager::getComponent(size_t entity) {
  size_t hash = typeid(T).hash_code();
  auto it = componentListMap_.find(hash);

  if (it == componentListMap_.end()) {
    return std::nullopt; // Component type not found
  }

  ComponentList<T>* componentList = static_cast<ComponentList<T>*>(it->second.get());

  if (entity >= componentList->componentlist.size() || !componentList->componentlist[entity].has_value()) {
    return std::nullopt; // Entity doesn't have this component or is out of range
  }

  return &componentList->componentlist[entity].value(); // Return a pointer to the component
}

/**
 * @brief Adds a component of the specified type to an entity.
 *
 * @tparam T The component type to add.
 * @param entity The ID of the entity to add the component to.
 * @return 0 on success, -1 on failure.
 */
template<typename T>
int ECSManager::addComponent(size_t entity)
{
  size_t hash = typeid(T).hash_code();
  map_type::iterator it = componentListMap_.find(hash);
  if (it == componentListMap_.end()) {
    std::cerr << "ERROR: Component list not found";
    return -1;
  }
  ComponentList<T>* cl = static_cast<ComponentList<T>*>(it->second.get());
  if (entity >= cl->componentlist.size()) {
    cl->grow(nextEntity);
  }

  cl->componentlist[entity] = T{};
  return 0;
}

/**
 * @brief Edits a component of the specified type for an entity.
 *
 * @tparam T The component type to edit.
 * @param entity The ID of the entity to edit the component for.
 * @param editor A function that modifies the component.
 * @return true if the component was edited successfully, false otherwise.
 */
template <typename T>
bool ECSManager::editComponent(size_t entity, const std::function<void(T&)>& editor) {
  size_t hash = typeid(T).hash_code();
  auto it = componentListMap_.find(hash);
  if (it == componentListMap_.end()) {
    std::cerr << "ERROR: Component type not found\n";
    return false;
  }

  ComponentList<T>* componentList = static_cast<ComponentList<T>*>(it->second.get());

  // Expand the vector if the entity exceeds the current size
  if (entity >= componentList->componentlist.size()) {
    componentList->grow((unsigned int)entity + 1); // Grow to include the entity
  }

  // Initialize the component if it's null
  if (!componentList->componentlist[entity].has_value()) {
    componentList->componentlist[entity] = T{}; // Create a new default component
  }

  // Edit the component
  editor(componentList->componentlist[entity].value());
  return true;
}

/**
 * @brief Grows the component list to accommodate a specified number of entities.
 *
 * @tparam T The component type stored in this list.
 * @param current_entities The number of entities to ensure capacity for.
 */
template<typename T>
void ComponentList<T>::grow(unsigned int current_entities) {
  size_t current_size = componentlist.size();
  size_t component_size = sizeof(T);
  size_t new_size = 0;

  if (componentlist.empty())
  {
    new_size = 1;
  }
  else if (current_size == (component_size * current_entities)) {
    return;
  }
  else {
    new_size = component_size * current_entities;
  }
  componentlist.resize(new_size, std::nullopt);
}



#endif
