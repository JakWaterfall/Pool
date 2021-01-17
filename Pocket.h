#pragma once
#include "Ball.h"
class Pocket : public Ball
{
	static constexpr int RADIUS = 20;

public:
	Pocket(float _x, float _y);

	void update(std::vector<Ball*>& balls);
	void eventHandler(SDL_Event* e);
private:
	void objectCollision(std::vector<Ball*>& balls);
};