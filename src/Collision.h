#ifndef COLLISION_H
#define COLLISION_H

#include <C:/libsdl/SDL2/include/SDL2/SDL.h>

class Collision {
    public:
        static bool CheckRectCollision(const SDL_Rect& rectA, const SDL_Rect& rectB);
};

#endif