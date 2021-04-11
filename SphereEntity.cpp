#include "SphereEntity.h"

SDL_Texture* SphereEntity::blackTexture = NULL;
SDL_Texture* SphereEntity::whiteTexture = NULL;
SDL_Texture* SphereEntity::redTexture = NULL;
SDL_Texture* SphereEntity::yellowTexture = NULL;
SDL_Texture* SphereEntity::pocketTexture = NULL;
SDL_Texture* SphereEntity::decorTexture = NULL;

SphereEntity::SphereEntity(float _x, float _y, Colours colour, int _radius)
	: position({ _x, _y }), radius(_radius), colour(colour)
{

	switch (colour)
	{
	case SphereEntity::Colours::white:
		r = 0x00; g = 0x00, b = 0x00;
		texture = SphereEntity::whiteTexture;
		break;
	case SphereEntity::Colours::black:
		r = 0x00; g = 0x00, b = 0x00;
		texture = SphereEntity::blackTexture;
		break;
	case SphereEntity::Colours::red:
		r = 0xFF; g = 0x00, b = 0x00;
		texture = SphereEntity::redTexture;
		break;
	case SphereEntity::Colours::yellow:
		r = 0xFF; g = 0xFF, b = 0x00;
		texture = SphereEntity::yellowTexture;
		break;
	case SphereEntity::Colours::pocket:
		r = 0x00; g = 0x00, b = 0x00;
		texture = SphereEntity::pocketTexture;
		break;
	case SphereEntity::Colours::decor:
		r = 0x00; g = 0x00, b = 0x00;
		texture = SphereEntity::decorTexture;
		break;
	default:
		r = 0x00; g = 0xFF, b = 0x00; // debug green so i know something went wrong
		break;
	}
}

// maybe change parameter for no. of repetitions 
void SphereEntity::render(SDL_Renderer* renderer)
{
	SDL_Rect ball = { (int)position.getX()-radius, (int)position.getY() - radius, (int)radius*2, (int)radius*2 };
	SDL_RenderCopy(renderer, texture, NULL, &ball);
}

void SphereEntity::renderWireframe(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);
	for (auto i = 0; i < 2; i++)
	{
		drawCirle(renderer, position, radius - i);
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

void SphereEntity::setTextures(SDL_Texture* black, SDL_Texture* white, SDL_Texture* red, SDL_Texture* yellow, SDL_Texture* pocket, SDL_Texture* decor)
{
	blackTexture = black;
	whiteTexture = white;
	redTexture = red;
	yellowTexture = yellow;
	pocketTexture = pocket;
	decorTexture = decor;
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

void SphereEntity::destroyTextures()
{
	if (whiteTexture)
	{
		SDL_DestroyTexture(whiteTexture);
		whiteTexture = NULL;
	}
	if (blackTexture)
	{
		SDL_DestroyTexture(blackTexture);
		blackTexture = NULL;
	}
	if (redTexture)
	{
		SDL_DestroyTexture(redTexture);
		redTexture = NULL;
	}
	if (yellowTexture)
	{
		SDL_DestroyTexture(yellowTexture);
		yellowTexture = NULL;
	}
	if (pocketTexture)
	{
		SDL_DestroyTexture(pocketTexture);
		pocketTexture = NULL;
	}
	if (decorTexture)
	{
		SDL_DestroyTexture(decorTexture);
		decorTexture = NULL;
	}
}