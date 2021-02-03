#pragma once
#include "WhiteBall.h"

class Players
{
	SphereEntity::Colours red = SphereEntity::Colours::red;
	SphereEntity::Colours yellow = SphereEntity::Colours::yellow;
	SphereEntity::Colours black = SphereEntity::Colours::black;
	SphereEntity::Colours white = SphereEntity::Colours::white;

	bool foulBall;
	bool arePlayerColoursSetup;
	bool isPlayer1Turn;
	int player1ShotsLeft;
	int player2ShotsLeft;
	SphereEntity::Colours player1Colour;
	SphereEntity::Colours player2Colour;

public:
	struct saveVariables
	{
		bool arePlayerColoursSetup;
		bool isPlayer1Turn;
		int player1ShotsLeft;
		int player2ShotsLeft;
		SphereEntity::Colours player1Colour;
		SphereEntity::Colours player2Colour;
	};

public:
	Players();

	void update(WhiteBall & white, std::vector<Ball>& pottedBalls);
	void render(SDL_Renderer* renderer);

	// Getters / Setter from file
	saveVariables getSaveVariables();
	void setVariablesFromFile(saveVariables saveVar);

private:
	void whiteHitOrMissOtherBall(WhiteBall& white);
	void resolvePottedBalls(std::vector<Ball>& pottedBalls);
	void setupColours(std::vector<Ball>& pottedBalls);
	void resolveFoulBall();
	void resolvePlayerTurn();

	void debugConsoleLogInfo();
};
