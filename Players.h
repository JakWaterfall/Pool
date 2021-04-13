#pragma once
#include "WhiteBall.h"
#include <SDL_ttf.h>
#include <string>

class Players
{
	struct Player
	{
		int ShotsLeft;
		SphereEntity::Colours Colour;
	};
	SphereEntity::Colours red = SphereEntity::Colours::red;
	SphereEntity::Colours yellow = SphereEntity::Colours::yellow;
	SphereEntity::Colours black = SphereEntity::Colours::black;
	SphereEntity::Colours white = SphereEntity::Colours::white;

	bool foulBall;
	bool arePlayerColoursSetup;
	bool isPlayer1Turn;
	Player player1;
	Player player2;

	bool gameOver;
	bool player1Won;

	TTF_Font* font = NULL;
	//SDL_Color textColor = { 0, 0, 0 };
	SDL_Surface* textSurface = NULL;
	SDL_Texture* textTexture = NULL;

	std::vector<Ball> displayBalls;

public:
	struct saveVariables
	{
		bool arePlayerColoursSetup;
		bool isPlayer1Turn;
		Player player1;
		Player player2;
		std::vector<Ball> displayBalls;
	};

public:
	Players();
	~Players();

	void update(WhiteBall& white, std::vector<Ball>& pottedBalls, std::vector<Ball*>& balls);
	void render(SDL_Renderer* renderer);

	// Getters / Setter from file
	saveVariables getSaveVariables();
	void setVariablesFromFile(saveVariables saveVar);
	bool getGameOver();

private:
	void whiteHitOrMissOtherBall(WhiteBall& white, std::vector<Ball*>& balls);
	void resolvePottedBalls(std::vector<Ball>& pottedBalls, std::vector<Ball*>& balls, WhiteBall& whiteBall);
	void setupColours(std::vector<Ball>& pottedBalls, SphereEntity::Colours& firstBallColour);
	void resolvePlayerTurn();
	void resolveWinner(std::vector<Ball>& pottedBalls, std::vector<Ball*>& balls, WhiteBall& whiteBall, int blackIndex);
	void renderText(SDL_Renderer* renderer, const char* text, int x, int y, SDL_Color textColour = { 0, 0, 0 });
	void renderDisplayBalls(SDL_Renderer* renderer);

	Player& getCurrentPlayer();
	Player& getOtherPlayer();

	void debugConsoleLogInfo();
	std::string getColourString();
};
