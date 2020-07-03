#include <iostream>
#include "./Constants.h"
#include "./Game.h"
#include "AssetManager.h"
#include "./Map.h"
#include "./Components/TransformComponent.h"
#include "./Components/SpriteComponent.h"
#include "./Components/ColliderComponent.h"
#include "../lib/glm/glm.hpp"
#include "./Components/KeyboardControlComponent.h"
#include "./Components/TextLabelComponent.h"
#include "./Components/ProjectileEmitterComponent.h"


EntityManager manager;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Renderer* Game::renderer;
SDL_Event Game::event;
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Map* map;

Game::Game() {
    this->isRunning = false;
}

Game::~Game() {

}

bool Game::IsRunning() const {
    return this->isRunning;
}

void Game::Initialize(int width, int height) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error init SDL." << std::endl;
        return;
    }
    if (TTF_Init() != 0) {
        std::cerr << "Error init SDL TTF" << std::endl;
    }
    window = SDL_CreateWindow(
        NULL, 
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_BORDERLESS
    );
    if (!window) {
        std::cerr << "Error creating window." << std::endl;
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        std::cerr << "Error creating renderer." << std::endl;
        return;
    }
    
    LoadLevel(0);

    isRunning = true;
    return;
}

Entity& player(manager.AddEntity("chopper", PLAYER_LAYER));

void Game::LoadLevel(int levelNumber) {
    std::string textureFilePath1 = "./assets/images/tank-big-right.png";
    assetManager->AddTexture("tank-image", textureFilePath1.c_str());
    std::string textureFilePath2 = "./assets/images/chopper-spritesheet.png";
    assetManager->AddTexture("chopper-image", textureFilePath2.c_str());
    assetManager->AddTexture("jungle", std::string("./assets/tilemaps/jungle.png").c_str());
    assetManager->AddFont("charriot-font", std::string("./assets/fonts/arial.ttf").c_str(), 14);
    assetManager->AddTexture("heliport-image", std::string("./assets/images/heliport.png").c_str());
    assetManager->AddTexture("projectile-image", std::string("./assets/images/bullet-enemy.png").c_str()); 

    map = new Map("jungle", 1, 32);
    map->LoadMap("./assets/tilemaps/jungle.map", 25, 20);

    Entity& tankEntity(manager.AddEntity("tank", ENEMY_LAYER));
    tankEntity.AddComponent<TransformComponent>(150, 495, 0, 0, 32, 32, 1);
    tankEntity.AddComponent<SpriteComponent>("tank-image");
    tankEntity.AddComponent<ColliderComponent>("ENEMY", 150, 495, 32, 32);

    //Entity& player = manager.AddEntity("chopper", PLAYER_LAYER);
    player.AddComponent<TransformComponent>(240, 106, 0, 0, 32, 32, 1);
    player.AddComponent<SpriteComponent>("chopper-image", 2, 90, true, false);
    player.AddComponent<KeyboardControlComponent>("up", "right", "down", "left", "space");
    player.AddComponent<ColliderComponent>("PLAYER", 240, 106, 32, 32);

    Entity& heliport(manager.AddEntity("Heliport", OBSTACLE_LAYER));
    heliport.AddComponent<TransformComponent>(470, 420, 0, 0, 32, 32, 1);
    heliport.AddComponent<SpriteComponent>("heliport-image");
    heliport.AddComponent<ColliderComponent>("LEVEL_COMPLETE", 470, 420, 32, 32);
    
    Entity& labelLevelName = manager.AddEntity("LabelLevelName", UI_LAYER);
    labelLevelName.AddComponent<TextLabelComponent>(10, 10, "First Level", "charriot-font", WHITE_COLOR);

    Entity& projectile(manager.AddEntity("projectile", PROJECTILE_LAYER));
    projectile.AddComponent<TransformComponent>(150+16, 495+16, 0, 0, 4, 4, 1);
    projectile.AddComponent<SpriteComponent>("projectile-image");
    projectile.AddComponent<ColliderComponent>("PROJECTILE", 150+16, 495+16, 4, 4);
    projectile.AddComponent<ProjectileEmitterComponent>(50, 270, 200, true);
}

void Game::ProcessInput() {
    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) {
            isRunning = false;
        }
        break;
    default:
        break;
    }
}

void Game::Update() {
    //wait until the target frame rate is reached.
    int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - ticksLastFrame);
    if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
        SDL_Delay(timeToWait);
    }

    /*
    deltTime is the difference in tick from last frame 
    to current frame, converted to seconds.
    */
    float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;

    //Clamp deltaTime to a maximum value
    deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;

    //Sets the new ticks for the current frame to be used for next pass.
    ticksLastFrame = SDL_GetTicks();

    manager.Update(deltaTime);

    HandleCameraMovement();
    CheckCollisions();
}

void Game::Render() {
    
    SDL_SetRenderDrawColor(renderer,21, 21, 21, 255);
    SDL_RenderClear(renderer);

    if (manager.HasNoEntities()) {
        return;
    }
    manager.Render();

    SDL_RenderPresent(renderer);
}

void Game::HandleCameraMovement() {
    TransformComponent* mainPlayerTransform = player.GetComponent<TransformComponent>();

    camera.x = mainPlayerTransform->position.x - (WINDOW_WIDTH / 2);
    camera.y = mainPlayerTransform->position.y - (WINDOW_HEIGHT / 2);

    camera.x = camera.x < 0 ? 0 : camera.x;
    camera.y = camera.y < 0 ? 0 : camera.y;
    camera.x = camera.x > camera.w ? camera.w : camera.x;
    camera.y = camera.y > camera.h ? camera.h : camera.y;
}

void Game::CheckCollisions() {
    CollisionType colType = manager.CheckCollisions();
    if (colType == PLAYER_ENEMY_COLLISION) {
        ProcessGameOver();
    }
    if (colType == PLAYER_PROJECTILE_COLLISION) {
        ProcessGameOver();
    }
    if (colType == PLAYER_LEVEL_COMPLETE_COLLISION) {
        ProcessNextLevel(1);
    }
}

void Game::ProcessGameOver() {
    std::cout << "Game Over" << std::endl;
    isRunning = false;
}

void Game::ProcessNextLevel(int levelNumber) {
    std::cout << "Next Level" << std::endl;
    isRunning = false;
}

void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}