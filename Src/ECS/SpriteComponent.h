#pragma once

// Include necessary headers
#include "Components.h"         // Include Components.h
#include "SDL.h"                // Include SDL.h
#include "../TextureManager.h"  // Include TextureManager.h

// Definition of SpriteComponent class
class SpriteComponent : public Component
{
private:
    TransformComponent* transform;
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;

public:
    // Default constructor
    SpriteComponent() = default;

    // Constructor with a path parameter to set the texture
    SpriteComponent(const char* path)
    {
        setTex(path);
    }

    // Destructor
    ~SpriteComponent()
    {
        SDL_DestroyTexture(texture);
    }

    // Function to set the texture
    void setTex(const char* path)
    {
        texture = TextureManager::LoadTexture(path);
    }

    // Initialization function
    void init() override
    {
        // Get a reference to the TransformComponent of the entity
        transform = &entity->getComponent<TransformComponent>();

        // Set the source rectangle for rendering
        srcRect.x = srcRect.y = 0;
        srcRect.w = transform->width;
        srcRect.h = transform->height;
    }

    // Update function
    void update() override
    {
        // Update the destination rectangle for rendering
        destRect.x = static_cast<int>(transform->position.x);
        destRect.y = static_cast<int>(transform->position.y);
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;
    }

    // Draw function
    void draw() override
    {
        // Draw the texture using the source and destination rectangles
        TextureManager::Draw(texture, srcRect, destRect);
    }
};
