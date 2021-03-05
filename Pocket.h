#pragma once
#include "SphereEntity.h"
#include "Ball.h"
#include <SDL_mixer.h>

class Pocket : public SphereEntity
{
	static Mix_Chunk* pocketSoundEffect;

public:
	Pocket(float _x, float _y, int radius);

	void update(std::vector<Ball*>& balls, std::vector<Ball>& pottedBalls);
	static void setSoundEffects(Mix_Chunk* pocketed);
	static void destroySoundEffects();
private:
	void objectCollision(std::vector<Ball*>& balls, std::vector<Ball>& pottedBalls);
};