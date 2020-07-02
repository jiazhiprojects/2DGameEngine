#ifndef ENTITY_H
#define ENTITY_H

#include "./Constants.h"
#include <vector>
#include <string>
#include <map>

class Component;
class EntityManager;

class Entity {
    private:
        EntityManager& manager;
        bool isActive;
        std::vector<Component*> components;
        std::map<const std::type_info*, Component*> typeToComponentMap;
    public:
        std::string name;
        LayerType layer;
        Entity(EntityManager& manager);
        Entity(EntityManager& manager, std::string name, LayerType layer);
        void Update(float deltaTime);
        void Render();
        void Destroy();
        bool IsActive() const;

        template <typename T, typename... TArgs>
        T& AddComponent(TArgs&&... args) {
            T* newComponent(new T(std::forward<TArgs>(args)...));
            newComponent->owner = this;
            components.emplace_back(newComponent);
            typeToComponentMap[&typeid(*newComponent)] = newComponent;
            newComponent->Initialize();
            return *newComponent;
        }

        template <typename T>
        T* GetComponent() {
            return static_cast<T*>(typeToComponentMap[&typeid(T)]);
        }

        template <typename T>
        bool HasComponent() const {
            return typeToComponentMap.count(&typeid(T));
        }
};

#endif