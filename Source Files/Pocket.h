#pragma once
#include <SDL_mixer.h>

#include "SphereEntity.h"
#include "Ball.h"

class Pocket : public SphereEntity
{
private:
	static Mix_Chunk* pocketSoundEffect;

public:
	// Constructor
	Pocket(float _x, float _y, int radius);

	//Functions
	void update(std::vector<Ball*>& balls, std::vector<Ball>& pottedBalls);
	static void setSoundEffects(Mix_Chunk* pocketed);
	static void destroySoundEffects();
private:
	void objectCollision(std::vector<Ball*>& balls, std::vector<Ball>& pottedBalls);
};