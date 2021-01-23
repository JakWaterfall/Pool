#pragma once
#include "Ball.h"
class WhiteBall : public Ball
{
	bool dropBall; // MAKE BETTER IMPLIMENTATION THEN USING RADIUS
	Vector mouse;
	float aimerMaxLenght = 200;
public:
	WhiteBall(float _x, float _y, bool dropBall, SphereEntity::Colours colour = SphereEntity::Colours::white);

	void update(std::vector<Ball*>& balls);
	void render(SDL_Renderer* renderer);
	void eventHandler(SDL_Event* e);
	void droppingBall(SDL_Event* e);
	void dropBallCollision();
	void markForDelete(std::vector<Ball*>& balls);
};

