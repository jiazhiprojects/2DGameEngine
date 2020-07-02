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

CollisionType EntityManager::CheckCollisions() const {
    for (int i = 0; i < entities.size() - 1; i++) {
        auto& thisEntity = entities[i];
        if (thisEntity->HasComponent<ColliderComponent>()) {
            ColliderComponent* thisCol = thisEntity->GetComponent<ColliderComponent>();
            for (int j = i + 1; j < entities.size(); j++) {
                auto& otherEntity = entities[j];
                if (otherEntity->name.compare("Tile") != 0) {
                    ColliderComponent* otherCol = otherEntity->GetComponent<ColliderComponent>();
                    if (Collision::CheckRectCollision(thisCol->collider, otherCol->collider)) {
                        if (thisCol->colliderTag.compare("PLAYER") == 0 &&
                            otherCol->colliderTag.compare("ENEMY") == 0) {
                            return PLAYER_ENEMY_COLLISION;
                        }

                        if (thisCol->colliderTag.compare("PLAYER") == 0 &&
                            otherCol->colliderTag.compare("PROJECTILE") == 0) {
                                return PLAYER_PROJECTILE_COLLISION;
                        }  

                        if (thisCol->colliderTag.compare("PLAYER") == 0 &&
                            otherCol->colliderTag.compare("LEVEL_COMPLETE") == 0) {
                                return PLAYER_LEVELCOMPLETE_COLLISION;
                        } 

                    // if (thisCol->colliderTag.compare("ENEMY") == 0 &&
                    //     otherCol->colliderTag.compare("FRIENDLY_PROJECTILE")) {
                    //         return ENEMY_;
                    // }                 
                    }
                }

            }
        }
    }
    return NO_COLLISION;
}

unsigned int EntityManager::GetEntityCount() {
    return entities.size();
}
