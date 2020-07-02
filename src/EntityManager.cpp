#include "./EntityManager.h"
#include "./Components/ColliderComponent.h"
#include "./Collision.h"

void EntityManager::ClearData() {
    for (auto& entity: entities) {
        entity->Destroy();
    }
}

bool EntityManager::HasNoEntities() const {
    return entities.size() == 0;
} 

void EntityManager::Update(float deltaTime) {
    for (auto& entity:entities) {
        entity->Update(deltaTime);
    }
}

void EntityManager::Render() {
    // for (auto& entity: entities) {
    //    entity->Render(); 
    // }
    for (int layerNumber = 0; layerNumber < NUM_LAYERS; layerNumber++) {
        for (auto& entity: GetEntitiesByLayer(static_cast<LayerType>(layerNumber))) {
            entity->Render();
        }
    }
}

Entity& EntityManager::AddEntity(std::string entityName, LayerType layer) {
    Entity *entity = new Entity(*this, entityName, layer);
    entities.emplace_back(entity);
    return *entity;
}

std::vector<Entity*> EntityManager::GetEntities() const {
    return entities;
}

std::vector<Entity*> EntityManager::GetEntitiesByLayer(LayerType layer) const {
    std::vector<Entity*> selectedEntities;
    for (auto& entity: entities) {
        if (entity->layer == layer) {
            selectedEntities.emplace_back(entity);
        }
    }
    return selectedEntities;
}

std::string EntityManager::CheckEntityCollisions(Entity& myEntity) const {
    ColliderComponent* myCollider = myEntity.GetComponent<ColliderComponent>();
    for (auto& entity: entities) {
        if (entity->name.compare(myEntity.name) != 0 && entity->name.compare("Tile") != 0) {
            if (entity->HasComponent<ColliderComponent>()) {
                ColliderComponent* other = entity->GetComponent<ColliderComponent>();
                if (Collision::CheckRectCollision(myCollider->collider, other->collider)) {
                    return other->colliderTag;
                }
            }
        }

    }
    return std::string();
}

unsigned int EntityManager::GetEntityCount() {
    return entities.size();
}
