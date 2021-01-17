#pragma once
#include <SDL.h>
#include <iostream>
#include <chrono>

#include "Constants.h"
#include "Ball.h"

class GameEngine
{

	SDL_Window* win = NULL;
	SDL_Surface* winSurface = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Event e;

	bool running;

	std::vector<Ball> balls;

public:
	//Constuctors
	GameEngine();

private:
	// Functions
	void run();
	void render();
	void renderBackground();
	void update();
	void eventHandler();
	void quit();
	SDL_Surface* loadImage(const char* filePath);


};

