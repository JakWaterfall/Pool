#include "Pocket.h"

Pocket::Pocket(float _x, float _y): SphereEntity(_x, _y, SphereEntity::Colours::black, RADIUS)
{
}

void Pocket::update(std::vector<Ball*>& balls, std::vector<Ball>& pottedBalls)
{
	objectCollision(balls, pottedBalls);
}


void Pocket::objectCollision(std::vector<Ball*>& balls, std::vector<Ball>& pottedBalls)
{
	for (auto& b : balls)
	{
		Vector test = position - b->getPosition();
		float dist = test.magnitude();
		if (dist < radius + b->getRadius() && b->getIsInteractable())  // this is so when the white ball is being dropped it dosent hit the other balls
		{
			pottedBalls.push_back(*b);
			b->potted(balls);
		}
	}
}
