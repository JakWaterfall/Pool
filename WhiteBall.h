#pragma once
#include "Ball.h"
class WhiteBall : public Ball
{
	struct Info
	{
		bool endTurn;
		bool hit;
		bool hitOtherBall;
		SphereEntity::Colours colourHitFirst;
	};
	Vector mouse;
	bool dropBall; 
	float aimerMaxLenght = 200;
	float maxHitStrength = 10;

	bool collideWithBall = false;
	bool anyBallsMoving = false;

public:
	Info info;

private:
	bool willCollideWithBall(std::vector<Ball*>& balls);
	bool mouseWithinDropBallArea();
	bool checkIfballsMoving(std::vector<Ball*>& balls);
	void ballCollision(std::vector<Ball*>& balls);
public:
	WhiteBall(float _x, float _y, bool dropBall, SphereEntity::Colours colour = SphereEntity::Colours::white);


	void update(std::vector<Ball*>& balls);
	void render(SDL_Renderer* renderer);
	void eventHandler(SDL_Event* e);
	void droppingBall(SDL_Event* e);
	void keepInDropBallArea();
	void potted();
};

