#include "GameObject.h"
#include "TextureManager.h"

// Constructor for GameObject
GameObject::GameObject(const char* textureSheet, int x, int y)
{
	// Load the texture for the GameObject
	objTexture = TextureManager::LoadTexture(textureSheet);

	// Set initial position
	xpos = x;
	ypos = y;
}

// Update function for GameObject
void GameObject::Update()
{
	// Update position and size of source rectangle
	xpos++;
	ypos++;
	srcRect.h = 32;
	srcRect.w = 32;
	srcRect.x = 0;
	srcRect.y = 0;

	// Update position and size of destination rectangle
	destRect.x = xpos;
	destRect.y = ypos;
	destRect.w = srcRect.w * 2;
	destRect.h = srcRect.h * 2;
}

// Render function for GameObject
void GameObject::Render()
{
	// Render the GameObject by copying the texture to the destination rectangle
	SDL_RenderCopy(Game::renderer, objTexture, NULL, &destRect);
}
