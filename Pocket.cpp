#include "Pocket.h"

Pocket::Pocket(float _x, float _y): Ball(_x, _y, RADIUS, false)
{
	r = 0x00; g = 0x00, b = 0x00;
}

void Pocket::update(std::vector<Ball>& balls)
{
	objectCollision(balls);
}

void Pocket::eventHandler(SDL_Event* e)
{
}

void Pocket::objectCollision(std::vector<Ball>& balls)
{
	for (auto& b : balls)
	{
		int r = radius;
		Vector test = position - b.getPosition();
		float dist = test.magnitude();
		balls.erase(std::remove_if(balls.begin(), balls.end(), [dist, r](Ball& b) {return dist < r + b.getRadius();}), balls.end());
	}
}
