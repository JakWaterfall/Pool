#pragma once
#include <SDL.h>
#include<iostream>
#include <vector>
#include <cstdint>
#include "Constants.h"
#include "Vector.h"

class Ball
{	
	bool isRed;
	Vector mouse;

protected:
	Uint8 r;
	Uint8 g;
	Uint8 b;
	int radius;
	Vector position;
	Vector velocity;

public:
	Ball(float _x, float _y, int _radius, bool isRed);

public:
	virtual void update(std::vector<Ball> & balls);
	virtual void render(SDL_Renderer* renderer);
	virtual void eventHandler(SDL_Event* e);
	void gravToMouse(); // remove
	Vector& getPosition(){ return position; }
	int getRadius() { return radius; }

protected:
	void applyFriction();
	void wallCollision();
	void drawCirle(SDL_Renderer* renderer, Vector position, int radius);
	virtual void objectCollision(std::vector<Ball> & balls);

};

