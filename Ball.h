#pragma once
#include "SphereEntity.h"

class Ball : public SphereEntity
{	

	float frictionStrength = 0.03; // impliment super slippery mode??
protected:
	int ballCollisionStrenght = 2;
	Vector velocity;
	bool isInteractable = true;
	bool deleteFlag = false;

public:
	Ball(float _x, float _y, SphereEntity::Colours colour, int radius = 10);

public:
	virtual void update(std::vector<Ball*> & balls);
	virtual void eventHandler(SDL_Event* e);
	virtual void potted(std::vector<Ball*>& balls);
	bool getDeleteFlag();
	bool getIsInteractable() { return isInteractable; }
	Vector& getVelocity();

protected:
	void applyFriction();
	void wallCollision();
	bool ballMoving();
	virtual void ballCollision(std::vector<Ball*> & balls);
	virtual bool testCollision(Vector& v_FromBallToBall, Ball& b);

};

