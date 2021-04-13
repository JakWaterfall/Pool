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
		texture = SphereEntity::whiteTexture;
		break;
	case SphereEntity::Colours::black:
		texture = SphereEntity::blackTexture;
		break;
	case SphereEntity::Colours::red:
		texture = SphereEntity::redTexture;
		break;
	case SphereEntity::Colours::yellow:
		texture = SphereEntity::yellowTexture;
		break;
	case SphereEntity::Colours::pocket:
		texture = SphereEntity::pocketTexture;
		break;
	case SphereEntity::Colours::decor:
		texture = SphereEntity::decorTexture;
		break;
	default:
		break;
	}
}

void SphereEntity::render(SDL_Renderer* renderer)
{
	SDL_Rect ball = { (int)position.getX()-radius, (int)position.getY() - radius, (int)radius*2, (int)radius*2 };
	SDL_RenderCopy(renderer, texture, NULL, &ball);
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

bool SphereEntity::getIsInteractable()
{
	return isInteractable; 
}

SDL_Texture* SphereEntity::getTexture()
{
	return texture;
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