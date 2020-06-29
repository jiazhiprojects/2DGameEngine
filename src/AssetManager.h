#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <map>
#include <string>
#include "./EntityManager.h"
#include "./TextureManager.h"

//#include "./Game.h"
class AssetManager {
    private:
        EntityManager* manager;
        std::map<std::string, SDL_Texture*> textures;

    public:
        AssetManager(EntityManager* manager);
        ~AssetManager();
        void ClearData();
        void AddTexture(std::string textureId, const char* filePath);
        SDL_Texture* GetTexture(std::string textureId);
};

#endif