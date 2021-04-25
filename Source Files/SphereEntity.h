#pragma once
#include <SDL.h>
#include <iostream>
#include <vector>

#include "Constants.h"
#include "Vector.h"

class SphereEntity
{
public:
	enum class Colours //Enumeration of colours for the balls, pocket and decoration.
	{
		white = 0, black = 1, red = 2, yellow = 3, pocket = 4, decor = 5
	};

protected:
	Vector position;
	Colours colour;
	int radius;
	bool isInteractable = true;
	SDL_Texture* texture = NULL;

	static SDL_Texture* blackTexture;
	static SDL_Texture* whiteTexture;
	static SDL_Texture* redTexture;
	static SDL_Texture* yellowTexture;
	static SDL_Texture* pocketTexture;
	static SDL_Texture* decorTexture;

	// Constructor
	SphereEntity(float _x, float _y, Colours colour, int _radius = 10);

public:
	// Functions
	virtual void render(SDL_Renderer* renderer);
	Vector& getPosition();
	Colours getColour();
	int getRadius();
	bool getIsInteractable();
	SDL_Texture* getTexture();
	static void destroyTextures();
	static void setTextures(SDL_Texture* blackTexture, SDL_Texture* whiteTexture, SDL_Texture* redTexture,
		SDL_Texture* yellowTexture, SDL_Texture* pocket, SDL_Texture* decor);

protected:
	bool testCollision(Vector& v_FromBallToBall, SphereEntity& object);
};
