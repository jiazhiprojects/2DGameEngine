#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "./EntityManager.h"
#include "./Component.h"
#include "./Entity.h"

class Game {
    private:
        bool isRunning;
        SDL_Window* window;
        
    public:
        Game();
        ~Game();
        static SDL_Renderer* renderer;
        void LoadLevel(int levelNumber);
        int ticksLastFrame;
        bool IsRunning() const;
        void Initialize(int width, int height);
        void ProcessInput();
        void Update();
        void Render();
        void Destroy();

};

#endif