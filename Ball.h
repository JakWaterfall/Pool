#pragma once
#include <SDL.h>
#include<iostream>
#include <vector>
#include "Constants.h"
#include "Vector.h"

class Ball
{
	float mass;
	Vector position;
	int radius;
	Vector velocity;
	Vector mouse;
	Vector acc;
	Vector gravity;

	Vector center;

public:
	Ball(float _x, float _y, int _radius, float mass);

public:
	void update(std::vector<Ball> & balls);
	void render(SDL_Renderer* renderer);
	void gravToMouse();
	void applyFriction();
	void eventHandler(SDL_Event* e);
	float getMass() { return mass; }

private:
	void wallCollision();
	void objectCollision(std::vector<Ball> & balls);
	void drawCirle(SDL_Renderer* renderer, Vector position, int radius);

};

