#pragma once
#include <chrono>
#include <fstream>
#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <windows.h>

#include "Ball.h"
#include "Constants.h"
#include "Players.h"
#include "Pocket.h"
#include "SphereEntity.h"
#include "WhiteBall.h"

class GameEngine
{
private:
	SDL_Window* win = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Event e;

	bool running;

	WhiteBall whiteBall;
	std::vector<Ball*> balls;
	std::vector<Pocket> pockets;
	std::vector<Ball> pottedBalls;

	Players* players;

	// Decorations
	SDL_Rect grayBackground;
	SDL_Rect dropBallAreaFelt;
	SDL_Rect tableFelt;
	SDL_Rect tableOutline;
	SDL_Rect tableWallLeft;
	SDL_Rect tableWallTop;
	SDL_Rect tableWallRight;
	SDL_Rect tableWallBottom;
	SDL_Rect tableWallOutline;

public:
	//Constructor
	GameEngine(bool resume, int pocketSize);

private:
	// Functions
	void deleteBalls();
	void eventHandler();
	void placeNewBalls();
	void quit();
	void render();
	void renderBackground();
	void run();
	void saveGameDialogue();
	void saveStateOfTable();
	void setupBallsAndPlayersFromFile();
	void update();
	SDL_Texture* loadTexture(const char* filePath);
};
