#pragma once
#include "SphereEntity.h"

class Ball : public SphereEntity
{	
protected:
	Vector velocity;
	bool deleteFlag = false;

public:
	Ball(float _x, float _y, SphereEntity::Colours colour, int radius = 10);

public:
	virtual void update(std::vector<Ball*> & balls);
	virtual void eventHandler(SDL_Event* e);
	virtual void markForDelete(std::vector<Ball*>& balls);
	bool getDeleteFlag();

protected:
	void applyFriction();
	void wallCollision();
	virtual void ballCollision(std::vector<Ball*> & balls);

};

