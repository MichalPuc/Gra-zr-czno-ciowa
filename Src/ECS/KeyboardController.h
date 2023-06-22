#pragma once

// Include necessary headers
#include "../Game.h"         // Include Game.h
#include "ECS.h"             // Include ECS.h
#include "Components.h"      // Include Components.h

// Definition of KeyboardController class
class KeyboardController : public Component
{
public:
    TransformComponent* transform;

    // Initialization function
    void init() override
    {
        // Get a reference to the TransformComponent of the entity
        transform = &entity->getComponent<TransformComponent>();
    }

    // Update function
    void update() override
    {
        // Check if a key is pressed
        if (Game::event.type == SDL_KEYDOWN)
        {
            switch (Game::event.key.keysym.sym)
            {
                // Update the velocity based on the pressed key
            case SDLK_w:
                transform->velocity.y = -1;
                break;
            case SDLK_a:
                transform->velocity.x = -1;
                break;
            case SDLK_d:
                transform->velocity.x = 1;
                break;
            case SDLK_s:
                transform->velocity.y = 1;
                break;
            default:
                break;
            }
        }

        // Check if a key is released
        if (Game::event.type == SDL_KEYUP)
        {
            switch (Game::event.key.keysym.sym)
            {
                // Update the velocity based on the released key
            case SDLK_w:
                transform->velocity.y = 0;
                break;
            case SDLK_a:
                transform->velocity.x = 0;
                break;
            case SDLK_d:
                transform->velocity.x = 0;
                break;
            case SDLK_s:
                transform->velocity.y = 0;
                break;
            default:
                break;
            }
        }
    }
};
