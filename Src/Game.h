#pragma once
#include "SDL.h"
#include<stdio.h>
#include"SDL_image.h"
#include <iostream>
#include <vector>

class ColliderComponent;

class Game
{
public:
	Game();
	~Game();
	int cnt = 0;
	void init(const char* title,int width, int height, int xpos, int ypos, bool fullscreen);
	void update();
	void render();
	void clean();
	void handleEvents();
	bool running() { return isRunning; }

	static void AddTile(int id, int x, int y);

	static SDL_Renderer* renderer;
	static SDL_Event event;

	static std::vector<ColliderComponent*> colliders;
private:
	bool isRunning;
	SDL_Window* window;
};

