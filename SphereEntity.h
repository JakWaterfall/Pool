#pragma once
#include <SDL.h>
#include<iostream> // possible remove
#include <vector>
#include <cstdint> // possible remove with rgb
#include "Constants.h"
#include "Vector.h"

class SphereEntity
{
public:
	enum class Colours
	{
		white = 0, black = 1, red = 2, yellow = 3, pocket = 4, decor = 5
	};
protected:
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Colours colour;
	int radius;
	Vector position;

	SDL_Texture* texture = NULL;

	static SDL_Texture* blackTexture;
	static SDL_Texture* whiteTexture;
	static SDL_Texture* redTexture;
	static SDL_Texture* yellowTexture;
	static SDL_Texture* pocketTexture;
	static SDL_Texture* decorTexture;
	
	// Constuctor
	SphereEntity(float _x, float _y, Colours colour, int _radius = 10);

public:
	virtual void render(SDL_Renderer* renderer);
	virtual void renderWireframe(SDL_Renderer* renderer);
	Vector& getPosition();
	int getRadius();
	Colours getColour();
	static void setTextures(SDL_Texture* blackTexture, SDL_Texture* whiteTexture, SDL_Texture* redTexture, 
							SDL_Texture* yellowTexture, SDL_Texture* pocket, SDL_Texture* decor);
	static void destroyTextures();

protected:
	void drawCirle(SDL_Renderer* renderer, Vector position, int radius);
};

