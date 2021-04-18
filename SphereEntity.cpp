#include "SphereEntity.h"

// Static textures initialised to NULL
SDL_Texture* SphereEntity::blackTexture = NULL;
SDL_Texture* SphereEntity::whiteTexture = NULL;
SDL_Texture* SphereEntity::redTexture = NULL;
SDL_Texture* SphereEntity::yellowTexture = NULL;
SDL_Texture* SphereEntity::pocketTexture = NULL;
SDL_Texture* SphereEntity::decorTexture = NULL;

/// <summary>
///	Constructor that sets the texture of the sphere based on the colour enum passed.
/// </summary>
/// <param name="_x">X value of position.</param>
/// <param name="_y">Y value of position.</param>
/// <param name="colour">Colour of sphere.</param>
/// <param name="_radius">Radius of sphere.</param>
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

/// <summary>
///	Draws the sphere to screen based on its x/y coordinates, radius and texture.
/// </summary>
/// <param name="renderer">SDL renderer object that draws to screen.</param>
void SphereEntity::render(SDL_Renderer* renderer)
{
	SDL_Rect ball = { (int)position.getX() - radius, (int)position.getY() - radius, (int)radius * 2, (int)radius * 2 };
	SDL_RenderCopy(renderer, texture, NULL, &ball);
}

/// <summary>
///	sets all the static textures for use by all of the sphere objects.
/// </summary>
/// <param name="black">Texture for Black Ball.</param>
/// <param name="white">Texture for White Ball.</param>
/// <param name="red">Texture for Red Ball.</param>
/// <param name="yellow">Texture for Yellow Ball.</param>
/// <param name="pocket">Texture for Pocket.</param>
/// <param name="decor">Texture for Decoration (semi circle on table).</param>
void SphereEntity::setTextures(SDL_Texture* black, SDL_Texture* white, SDL_Texture* red, SDL_Texture* yellow, SDL_Texture* pocket, SDL_Texture* decor)
{
	blackTexture = black;
	whiteTexture = white;
	redTexture = red;
	yellowTexture = yellow;
	pocketTexture = pocket;
	decorTexture = decor;
}

/// <summary>
/// Deletes all the textures and free's up the memory.
/// </summary>
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

/// <summary>
///	Tests if two sphere objects collided and returns a boolean.
/// Requires a reference to a 2Dvector to preform the calculation which can be used
/// later in the code that called the testCollision function. For example, the magnitude of the 2Dvector may be used.
/// </summary>
/// <param name="v_FromBallToBall">2DVector to preform the calculations on.</param>
/// <param name="sphere">Object to check a collision for.</param>
/// <returns>Returns a boolean on whether a collision happened.</returns>
bool SphereEntity::testCollision(Vector& v_FromBallToBall, SphereEntity& sphere)
{
	v_FromBallToBall = position - sphere.getPosition();	// Create a vector which points from one object to another.
	float dist = v_FromBallToBall.magnitude();			// return the magnitude of that vector to work out the distance between them.

	// Test to see if the distance between the 2 objects is greater then their radii, which would indicate the objects were colliding.
	// Check if isInteractable so when the white ball is being dropped it doesn't hit the other balls / pockets.
	if (dist < radius + sphere.getRadius() && sphere.getIsInteractable())
	{
		return true;
	}
	return false;
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