#include "Pocket.h"

Pocket::Pocket(float _x, float _y): Ball(_x, _y, false, RADIUS)
{
	r = 0x00; g = 0x00, b = 0x00;
}

void Pocket::update(std::vector<Ball*>& balls)
{
	objectCollision(balls);
}

void Pocket::eventHandler(SDL_Event* e)
{
}

void Pocket::objectCollision(std::vector<Ball*>& balls)
{
	for (auto& b : balls)
	{
		Vector test = position - b->getPosition();
		float dist = test.magnitude();
		if (dist < radius + b->getRadius() && b->getRadius() < 15)  // this is so when the white ball is being dropped it dosent hit the other balls
		{
			b->markForDelete(balls);
		}
	}
}
