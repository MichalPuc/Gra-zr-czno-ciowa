#pragma once

// Include necessary headers
#include "Components.h"     // Include Components.h
#include "../Vector2D.h"    // Include Vector2D.h

// Definition of TransformComponent structure
struct TransformComponent : public Component
{
public:
    Vector2D position;
    Vector2D velocity;
    int speed = 5;
    int height = 32;
    int width = 32;
    int scale = 2;

    // Default constructor
    TransformComponent()
    {
        position.Zero();
    }

    // Constructor with parameters for initial position (x, y)
    TransformComponent(float x, float y)
    {
        position.x = x;
        position.y = y;
    }

    // Constructor with parameter for scale (sc)
    TransformComponent(int sc)
    {
        position.Zero();
        scale = sc;
    }

    // Constructor with parameters for position (x, y), height (h), width (w), and scale (sc)
    TransformComponent(float x, float y, int h, int w, int sc)
    {
        position.x = x;
        position.y = y;
        height = h;
        width = w;
        scale = sc;
    }

    // Initialization function
    void init() override
    {
        velocity.x = 0;
        velocity.y = 0;
    }

    // Update function
    void update() override
    {
        position.x += velocity.x * speed;
        position.y += velocity.y * speed;
    }
};
