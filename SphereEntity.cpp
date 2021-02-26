#include "SphereEntity.h"

SphereEntity::SphereEntity(float _x, float _y, Colours colour, int _radius)
	: position({ _x, _y }), radius(_radius), colour(colour)
{
	switch (colour)
	{
	case SphereEntity::Colours::white:
		r = 0x00; g = 0x00, b = 0x00;
		break;
	case SphereEntity::Colours::black:
		r = 0x00; g = 0x00, b = 0x00;
		break;
	case SphereEntity::Colours::red:
		r = 0xFF; g = 0x00, b = 0x00;
		break;
	case SphereEntity::Colours::yellow:
		r = 0xFF; g = 0xFF, b = 0x00;
		break;
	default:
		r = 0x00; g = 0xFF, b = 0x00; // debug green so i know something went wrong
		break;
	}
}

// maybe change parameter for no. of repetitions 
void SphereEntity::render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);
	for (auto i = 0; i < 3; i++)
	{
		drawCirle(renderer, position, radius - i); // + or - ??
	}
}

Vector& SphereEntity::getPosition()
{
	return position;
}

int SphereEntity::getRadius()
{
	return radius;
}

SphereEntity::Colours SphereEntity::getColour()
{
	return colour;
}

void SphereEntity::drawCirle(SDL_Renderer* renderer, Vector position, int radius)
{
	// https://www.thecrazyprogrammer.com/2016/12/bresenhams-midpoint-circle-algorithm-c-c.html

	int x0 = (int)position.getX();
	int y0 = (int)position.getY();
	int x = radius;
	int y = 0;
	int err = 0;
	while (x >= y)
	{
		SDL_RenderDrawPoint(renderer, x0 + x, y0 + y);
		SDL_RenderDrawPoint(renderer, x0 + y, y0 + x);
		SDL_RenderDrawPoint(renderer, x0 - y, y0 + x);
		SDL_RenderDrawPoint(renderer, x0 - x, y0 + y);
		SDL_RenderDrawPoint(renderer, x0 - x, y0 - y);
		SDL_RenderDrawPoint(renderer, x0 - y, y0 - x);
		SDL_RenderDrawPoint(renderer, x0 + y, y0 - x);
		SDL_RenderDrawPoint(renderer, x0 + x, y0 - y);

		if (err <= 0)
		{
			y += 1;
			err += 2 * y + 1;
		}

		if (err > 0)
		{
			x -= 1;
			err -= 2 * x + 1;
		}
	}
}
