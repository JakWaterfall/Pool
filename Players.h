#pragma once
#include "WhiteBall.h"

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


public:
	struct saveVariables
	{
		bool arePlayerColoursSetup;
		bool isPlayer1Turn;
		Player player1;
		Player player2;
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
	void setupColours(std::vector<Ball>& pottedBalls, SphereEntity::Colours & firstBallColour);
	void resolveFoulBall();
	void resolvePlayerTurn();

	Player& getCurrentPlayer();
	Player& getOtherPlayer();

	void debugConsoleLogInfo();
};
