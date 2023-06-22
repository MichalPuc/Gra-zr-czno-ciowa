#pragma once
#include <string>
#include <SDL.h>
#include "Components.h"

class ColliderComponent : public Component
{
public:
	SDL_Rect collider; // Rectangle representing the collider's position and size
	std::string tag; // Tag to identify the collider

	TransformComponent* transform; // Pointer to the associated TransformComponent

	ColliderComponent(std::string t)
	{
		tag = t;
	}

	ColliderComponent(std::string t, int xpos, int ypos, int size)
	{
		tag = t;
		collider.x = xpos;
		collider.y = ypos;
		collider.h = collider.w = size;
	}

	void init() override
	{
		// Initialize the collider component
		// If the entity does not have a TransformComponent, add one
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override
	{
		// Update the collider's position and size
		// Skip updating if the collider's tag is "terrain"
		if (tag != "terrain")
		{
			collider.x = static_cast<int>(transform->position.x);
			collider.y = static_cast<int>(transform->position.y);
			collider.w = transform->width * transform->scale;
			collider.h = transform->height * transform->scale;
		}
	}
};
