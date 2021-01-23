#pragma once
#include <SDL.h>
#include<iostream>
#include <vector>
#include <cstdint>
#include "Constants.h"
#include "Vector.h"

class SphereEntity
{
public:
	enum class Colours
	{
		white, black, red, yellow
	};
protected:
	Uint8 r;
	Uint8 g;
	Uint8 b;
	int radius;
	Vector position;
	
	// Constuctor
	SphereEntity(float _x, float _y, Colours colour, int _radius = 10);

public:
	virtual void render(SDL_Renderer* renderer);
	Vector& getPosition();
	int getRadius();

protected:
	void drawCirle(SDL_Renderer* renderer, Vector position, int radius);
};

