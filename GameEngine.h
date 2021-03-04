#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <chrono>
#include <fstream>
#include <windows.h>

#include "Constants.h"
#include "SphereEntity.h"
#include "Ball.h"
#include "Pocket.h"
#include "WhiteBall.h"
#include "Players.h"

class GameEngine
{
	SDL_Window* win = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Event e;

	bool running;

	WhiteBall whiteBall;
	std::vector<Ball*> balls;
	std::vector<Pocket> pockets;
	std::vector<Ball> pottedBalls;

	Players* players;

public:
	//Constuctors
	GameEngine(bool resume);

private:
	// Functions
	void run();
	void render();
	void renderBackground();
	void update();
	void eventHandler();
	void quit();
	void deleteBalls();

	void placeNewBalls();
	void saveStateOfTable();
	void setupBallsAndPlayersFromFile();
	void saveGameDialog();

	SDL_Surface* loadImage(const char* filePath);
	SDL_Texture* loadTexture(const char* filePath);


};

