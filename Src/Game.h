#pragma once
#include "SDL.h"
#include<stdio.h>
#include"SDL_image.h"
#include <iostream>
#include <vector>
#include <random>
#include <cstdlib>
#include <thread>
#include <chrono>

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


	static SDL_Renderer* renderer;
	static SDL_Event event;



	enum groupLabels : std::size_t
	{
		groupMap,
		groupPlayers,
		groupColliders,
		groupEnemies
	};
private:
	bool isRunning;
	SDL_Window* window;
};

