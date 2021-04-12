#pragma once
#include "Ball.h"
#include "Pocket.h"
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
	bool collideWithPocket = false;
	bool anyBallsMoving = false;
	std::vector<Pocket> pockets;

public:
	Info info;

private:
	bool willCollideWithBall(std::vector<Ball*>& balls);
	bool willCollideWithPocket();
	bool mouseWithinDropBallArea();
	bool checkIfballsMoving(std::vector<Ball*>& balls);
	void ballCollision(std::vector<Ball*>& balls) override;
public:
	WhiteBall(float _x, float _y, bool dropBall, std::vector<Pocket> & pockets, SphereEntity::Colours colour = SphereEntity::Colours::white);


	void update(std::vector<Ball*>& balls) override;
	void render(SDL_Renderer* renderer) override;
	void eventHandler(SDL_Event* e);
	void droppingBall(SDL_Event* e);
	void keepInDropBallArea();
	void potted() override;
	void resetInfo();
};

