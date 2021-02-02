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
		white = 0, black = 1, red = 2, yellow = 3
	};
protected:
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Colours colour;
	int radius;
	Vector position;
	
	// Constuctor
	SphereEntity(float _x, float _y, Colours colour, int _radius = 10);

public:
	virtual void render(SDL_Renderer* renderer);
	Vector& getPosition();
	int getRadius();
	Colours getColour();

protected:
	void drawCirle(SDL_Renderer* renderer, Vector position, int radius);
};

