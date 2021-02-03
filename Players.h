#pragma once
#include "WhiteBall.h"

class Players
{
	SphereEntity::Colours red = SphereEntity::Colours::red;
	SphereEntity::Colours yellow = SphereEntity::Colours::yellow;
	SphereEntity::Colours black = SphereEntity::Colours::black;
	SphereEntity::Colours white = SphereEntity::Colours::white;

	bool areColoursSetup;
	bool giveOtherPlayer2Shots;
	bool player1Turn;
	int player1Shots;
	int player2Shots;
	SphereEntity::Colours player1Colour;
	SphereEntity::Colours player2Colour;
public:
	Players();

	void update(WhiteBall & white, std::vector<Ball>& pottedBalls);
	void render(SDL_Renderer* renderer);
private:
	void whiteHitOrMissOtherBall(WhiteBall& white);
	void resolvePottedBalls(std::vector<Ball>& pottedBalls);
	void setupColours(std::vector<Ball>& pottedBalls);
	void resolve2Shots();
};
// only evaluate after all balls have stopped moving
//if was hit and vel == 0 and didnt hit ball either way --shot and if didnt hit give 2 shots to other player
// hit and hit oppersite player ball 