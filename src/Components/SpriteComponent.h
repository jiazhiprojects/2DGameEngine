#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../Animation.h"
#include "../TextureManager.h"
#include "../AssetManager.h"
#include "./TransformComponent.h"
class SpriteComponent: public Component {
    private:
        TransformComponent* transform;
        SDL_Texture* texture;
        SDL_Rect sourceRect;
        SDL_Rect destRect;
        bool isAnimated;
        int numFrames;
        int animSpeed;
        bool isFixed;
        std::map<std::string, Animation> animations;
        std::string currentAnimName;
        unsigned int animIndex = 0;

    public:
        SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

        SpriteComponent(const char* assetTextureId) {
            isAnimated = false;
            isFixed = false;
            SetTexture(assetTextureId);
        }

        SpriteComponent(std::string id, int numFrames, int animSpeed, bool hasDirections, bool isFixed) {
            isAnimated = true;
            this->numFrames = numFrames;
            this->animSpeed = animSpeed;
            this->isFixed = isFixed;
            if (hasDirections) {
                Animation downAnim = Animation(0, numFrames, animSpeed);
                Animation rightAnim = Animation(1, numFrames, animSpeed);
                Animation leftAnim = Animation(2, numFrames, animSpeed);
                Animation upAnim = Animation(3, numFrames, animSpeed);
                animations.emplace("DownAnim", downAnim);
                animations.emplace("RightAnim", rightAnim);
                animations.emplace("LeftAnim", leftAnim);
                animations.emplace("UpAnim", upAnim);
                this->animIndex = 0;
                this->currentAnimName = "DownAnim";
            } else {
                Animation singleAnim = Animation(0, numFrames, animSpeed);
                animations.emplace("SingleAnimation", singleAnim);
                this->animIndex = 0;
                this->currentAnimName = "SingleAnimation";
            }
            Play(this->currentAnimName);
            SetTexture(id);
        }

        void Play(std::string animName) {
            numFrames = animations[animName].numFrames;
            animIndex = animations[animName].index;
            animSpeed = animations[animName].animSpeed;
            currentAnimName = animName;
        }

        void SetTexture(std::string assetTextureId) {
            texture = Game::assetManager->GetTexture(assetTextureId);
        }

        void Initialize() override {
                transform = owner->GetComponent<TransformComponent>();
                sourceRect.x = 0;
                sourceRect.y = 0;
                sourceRect.w = transform->width;
                sourceRect.h = transform->height;
        }

        void Update(float deltaTime) {
            if (isAnimated) {
                sourceRect.x = sourceRect.w * static_cast<int>((SDL_GetTicks() / animSpeed) % numFrames);
            }
            sourceRect.y = animIndex * transform->height;

            destRect.x = static_cast<int>(transform->position.x) - (isFixed ? 0 :  Game::camera.x);
            destRect.y = static_cast<int>(transform->position.y) - (isFixed ? 0 :  Game::camera.y);
            destRect.w = transform->width * transform->scale;
            destRect.h = transform->height * transform->scale;
        }

        void Render() override {
            TextureManager::Draw(texture, sourceRect, destRect, spriteFlip);
        }
};

#endif