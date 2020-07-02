#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <C:/libsdl/SDL2/include/SDL2/SDL.h>
#include "../Game.h"
#include "../EntityManager.h"
#include "../Components/TransformComponent.h"

class ColliderComponent: public Component {
    public:
        std::string colliderTag;
        SDL_Rect collider;
        SDL_Rect sourceRect;
        SDL_Rect destRect;
        TransformComponent* transform;

        ColliderComponent(std::string colliderTag, int x, int y, int width, int height) {
            this->colliderTag = colliderTag;
            this->collider = {x, y, width, height};
        }

        void Initialize() override {
            if (owner->HasComponent<TransformComponent>()) {
                transform = owner->GetComponent<TransformComponent>();
                sourceRect = {0, 0, transform->width, transform->height};
                destRect = {collider.x, collider.y, collider.w, collider.h};
            }

        }

        void Update(float deltaTime) override {
            collider.x = static_cast<int>(transform->position.x);
            collider.y = static_cast<int>(transform->position.y);
            collider.w = transform->width * transform->scale;
            collider.h = transform->height * transform->scale;

            destRect.x = collider.x - Game::camera.x;
            destRect.y = collider.y - Game::camera.y;
        }
};

#endif