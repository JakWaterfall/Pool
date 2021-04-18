#pragma once
#include "Ball.h"
#include "Pocket.h"
class WhiteBall : public Ball
{
private:
	struct Info // Info struct is sent as a message to the players class in order to infer what happen on each turn.
	{
		bool endTurn;							// Is true on end of turn
		bool hit;								// Is true when the white ball is hit
		bool hitOtherBall;						// Is true if the white ball hits another ball
		SphereEntity::Colours colourHitFirst;	// Is colour of ball that the white ball hit first
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

public:
	// Constructor
	WhiteBall(float _x, float _y, bool dropBall, std::vector<Pocket>& pockets, SphereEntity::Colours colour = SphereEntity::Colours::white);

	// Functions
	void update(std::vector<Ball*>& balls) override;
	void render(SDL_Renderer* renderer) override;
	void eventHandler(SDL_Event* e);
	void droppingBall(SDL_Event* e);
	void potted() override;
	void resetInfo();

private:
	bool willCollideWithBall(const std::vector<Ball*>& balls);
	bool willCollideWithPocket();
	bool mouseWithinDropBallArea();
	bool checkIfballsMoving(std::vector<Ball*>& balls);
	void keepInDropBallArea();
	void ballCollision(std::vector<Ball*>& balls) override;
};
