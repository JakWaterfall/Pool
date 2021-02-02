#pragma once
#include "WhiteBall.h"

class Players
{
	bool player1Turn;
	int player1Shots;
	int player2Shots;
	SphereEntity::Colours player1Colour;
	SphereEntity::Colours player2Colour;
public:
	Players();

	void update();
	void render();
};