#pragma once

// Include necessary headers
#include "ECS.h"                    // Include ECS.h
#include "TransformComponent.h"     // Include TransformComponent.h
#include "SDL.h"                    // Include SDL.h

// Definition of TileComponent class
class TileComponent : public Component
{
public:
    TransformComponent* transform;
    SpriteComponent* sprite;

    SDL_Rect tileRect;
    int tileID;
    char* path;

    // Default constructor
    TileComponent() = default;

    // Constructor with parameters for tile position, ID, tile size, and tile scale
    TileComponent(int x, int y, int id, int tsize, int tscale)
    {
        tileRect.x = x;
        tileRect.y = y;
        tileRect.w = tsize;
        tileRect.h = tsize;

        tileID = id;

        // Set the path based on the tile ID
        switch (tileID)
        {
        case 3:
            path = "images/water.png";
            break;
        case 2:
            path = "images/water.png";
            break;
        case 0:
            path = "images/dirt.png";
            break;
        case 1:
            path = "images/grass.png";
            break;
        default:
            break;
        }
    }

    // Initialization function
    void init() override
    {
        // Add a TransformComponent to the entity
        entity->addComponent<TransformComponent>((float)tileRect.x, (float)tileRect.y, tileRect.w, tileRect.h, 1);
        transform = &entity->getComponent<TransformComponent>();

        // Add a SpriteComponent to the entity
        entity->addComponent<SpriteComponent>(path);
        sprite = &entity->getComponent<SpriteComponent>();
    }
};
