#pragma once
#include "SphereEntity.h"
#include "Ball.h"

class Pocket : public SphereEntity
{
	static constexpr int RADIUS = 20;

public:
	Pocket(float _x, float _y);

	void update(std::vector<Ball*>& balls);
private:
	void objectCollision(std::vector<Ball*>& balls);
};