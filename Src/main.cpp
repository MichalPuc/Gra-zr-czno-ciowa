#include "Game.h"

// Create a pointer to the Game object
Game* game = nullptr;

int main(int argc, char* argv[])
{
	// Set the desired frames per second
	const int FPS = 60;

	// Calculate the delay between frames based on the desired FPS
	const int frameDelay = 1000 / FPS;

	// Variables to track frame timing
	Uint32 frameStart;
	int frameTime;

	// Create a new instance of the Game object
	game = new Game();

	// Initialize the game window with specified parameters
	game->init("GameWindow", 750, 810, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, false);

	// Game loop
	while (game->running())
	{
		// Get the starting time of the frame
		frameStart = SDL_GetTicks();

		// Handle user events
		game->handleEvents();

		// Update game logic
		game->update();

		// Render the game
		game->render();

		// Calculate the time taken for the frame
		frameTime = SDL_GetTicks() - frameStart;

		// Delay the execution if necessary to achieve the desired frame rate
		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	// Clean up resources and exit the game
	game->clean();
	return 0;
}
