#pragma once
#include "WhiteBall.h"
#include <SDL_ttf.h>
#include <string>

class Players
{
	struct Player // Player struct will hold the info for both players (Their colour and how many shots they have left)
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
	bool gameOver;
	bool player1Won;
	Player player1;
	Player player2;

	TTF_Font* font = NULL;
	SDL_Surface* textSurface = NULL;
	SDL_Texture* textTexture = NULL;

	std::vector<Ball> displayBalls;

public:
	struct saveVariables // Save variables struct packages the relevant data required to save and load from file.
	{
		bool arePlayerColoursSetup;
		bool isPlayer1Turn;
		Player player1;
		Player player2;
		std::vector<Ball> displayBalls;
	};

public:
	// Constructor / Destructor
	Players();
	~Players();

	// Functions
	void update(WhiteBall& white, std::vector<Ball>& pottedBalls, std::vector<Ball*>& balls);
	void render(SDL_Renderer* renderer);
	bool getGameOver();

	// Getters / Setter from file
	saveVariables getSaveVariables();
	void setVariablesFromFile(saveVariables saveVar);

private:
	void whiteHitOrMissOtherBall(WhiteBall& white, std::vector<Ball*>& balls);
	void resolvePottedBalls(std::vector<Ball>& pottedBalls, std::vector<Ball*>& balls, WhiteBall& whiteBall);
	void setupColours(std::vector<Ball>& pottedBalls, SphereEntity::Colours& firstBallColour);
	void resolvePlayerTurn();
	void resolveWinner(std::vector<Ball>& pottedBalls, std::vector<Ball*>& balls, WhiteBall& whiteBall, int blackIndex);
	void renderText(SDL_Renderer* renderer, const char* text, int x, int y, SDL_Color textColour = { 0, 0, 0 });
	void renderDisplayBalls(SDL_Renderer* renderer);
	std::string getColourString();

	Player& getCurrentPlayer();
	Player& getOtherPlayer();
};
