#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"

// Create a pointer to the game map and initialize the renderer
Map* map;
SDL_Renderer* Game::renderer = nullptr;

// Create an instance of the entity manager, SDL event, and a vector for movement
Manager manager;
SDL_Event Game::event;
Vector2D v;

// Create entities and groups using references
auto& player(manager.addEntity());
auto& enemy(manager.addEntity());
auto& enemy1(manager.addEntity());
auto& enemy2(manager.addEntity());
auto& colliders(manager.getGroup(Game::groupColliders));
auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& enemies(manager.getGroup(Game::groupEnemies));

// Game class constructor
Game::Game()
{

}

// Game class destructor
Game::~Game()
{

}

// Initialize the game
void Game::init(const char* title, int width, int height, int xpos, int ypos, bool fullscreen)
{
    // Initialize flags for the window creation
    int flags = 0;
    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    // Initialize SDL and create a window
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        isRunning = true;
    }
    else
    {
        isRunning = false;
    }

    // Load and set up the game map
    map = new Map(1, 48);
    map->LoadMap("images/16x19.map", 16, 19);

    // Add components and groups to the player entity
    player.addComponent<TransformComponent>(400, 320);
    player.addComponent<SpriteComponent>("./images/3.png");
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayers);

    // Add components and groups to the enemy entity
    enemy.addComponent<TransformComponent>(5, 5);
    enemy.addComponent<SpriteComponent>("./images/2.png");
    enemy.addComponent<ColliderComponent>("enemy");
    enemy.addGroup(groupEnemies);
    enemy.addGroup(groupColliders);

    // Add components and groups to the enemy2 entity
    enemy2.addComponent<TransformComponent>(650, 600);
    enemy2.addComponent<SpriteComponent>("./images/2.png");
    enemy2.addComponent<ColliderComponent>("enemy");
    enemy2.addGroup(groupEnemies);
    enemy2.addGroup(groupColliders);

    // Add components and groups to the enemy1 entity
    enemy1.addComponent<TransformComponent>(10, 400);
    enemy1.addComponent<SpriteComponent>("./images/2.png");
    enemy1.addComponent<ColliderComponent>("enemy");
    enemy1.addGroup(groupEnemies);
    enemy1.addGroup(groupColliders);
}

// Handle SDL events
void Game::handleEvents()
{
    // Poll for events and handle them
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        break;
    default:
        break;
    }
}

// Update the game state
void Game::update()
{
    // Get the collision rectangle and position of the player
    SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
    Vector2D playerPos = player.getComponent<TransformComponent>().position;

    // Get the collision rectangle and position of the enemy
    SDL_Rect enemyCol = enemy.getComponent<ColliderComponent>().collider;
    Vector2D enemyPos = enemy.getComponent<TransformComponent>().position;

    // Refresh and update the entity manager
    manager.refresh();
    manager.update();

    // Update the position of enemies randomly
    for (auto& e : enemies)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 4);

        int randomNumber = dis(gen);
        if (randomNumber == 1) v = Vector2D(1, 1);
        if (randomNumber == 2) v = Vector2D(1, -1);
        if (randomNumber == 3) v = Vector2D(-1, 1);
        if (randomNumber == 4) v = Vector2D(-1, -1);
        e->getComponent<TransformComponent>().position.Add(v);
    }

    // Check collision between player and colliders
    for (auto& c : colliders)
    {
        SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
        if (Collision::AABB(cCol, playerCol))
        {
            std::cout << "Przegrales";
            std::this_thread::sleep_for(std::chrono::seconds(2));
            clean();
            exit(0);
        }
    }
}

// Render the game
void Game::render()
{
    // Clear the renderer
    SDL_RenderClear(renderer);

    // Render tiles, players, and enemies
    for (auto& t : tiles)
    {
        t->draw();
    }
    for (auto& p : players)
    {
        p->draw();
    }
    for (auto& e : enemies)
    {
        e->draw();
    }

    // Present the rendered frame
    SDL_RenderPresent(renderer);
}

// Clean up resources
void Game::clean()
{
    // Destroy the window, renderer, and quit SDL
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game Cleaned" << std::endl;
}
