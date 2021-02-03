#pragma once
#include "WhiteBall.h"

class Players
{
	SphereEntity::Colours red = SphereEntity::Colours::red;
	SphereEntity::Colours yellow = SphereEntity::Colours::yellow;
	SphereEntity::Colours black = SphereEntity::Colours::black;
	SphereEntity::Colours white = SphereEntity::Colours::white;

	bool giveOtherPlayer2Shots;
	bool areColoursSetup;
	bool player1Turn;
	int player1Shots;
	int player2Shots;
	SphereEntity::Colours player1Colour;
	SphereEntity::Colours player2Colour;

public:
	struct saveVariables
	{
		bool areColoursSetup;
		bool player1Turn;
		int player1Shots;
		int player2Shots;
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
	void resolve2Shots();
};
// Change variable names