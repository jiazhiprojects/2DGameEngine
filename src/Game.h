#ifndef GAME_H
#define GAME_H

#include <C:/libsdl/SDL2/include/SDL2/SDL.h>
#include <C:/libsdl/SDL2/include/SDL2/SDL_image.h>
#include <C:/libsdl/SDL2/include/SDL2/SDL_ttf.h>
//#include "./AssetManager.h"
#include "./EntityManager.h"
#include "./Component.h"
#include "./Entity.h"

class AssetManager;

class Game {
    private:
        bool isRunning;
        SDL_Window* window;
        
    public:
        Game();
        ~Game();
        static SDL_Renderer* renderer;
        static AssetManager* assetManager;
        static SDL_Event event;
        static SDL_Rect camera;
        void LoadLevel(int levelNumber);
        int ticksLastFrame;
        bool IsRunning() const;
        void Initialize(int width, int height);
        void ProcessInput();
        void Update();
        void Render();
        void Destroy();
        void HandleCameraMovement();
        void CheckCollisions();

};

#endif