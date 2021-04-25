#include "Pocket.h"

Mix_Chunk* Pocket::pocketSoundEffect = NULL;

Pocket::Pocket(float _x, float _y, int radius) : SphereEntity(_x, _y, SphereEntity::Colours::pocket, radius)
{
}

/// Update function just calls the object collision function as all
/// the pockets job is to detect when a ball collides with them and pockets them.
void Pocket::update(std::vector<Ball*>& balls, std::vector<Ball>& pottedBalls)
{
	objectCollision(balls, pottedBalls);
}

/// Detects if a ball collides with the pocket and pockets them if true.
/// Marks the potted balls for removal for the vector (table).
///	Adds the potted balls to a vector for the player object to inspect and make logical decisions on.
void Pocket::objectCollision(std::vector<Ball*>& balls, std::vector<Ball>& pottedBalls) // change name from object collision
{
	Vector v_FromBallToPocket;

	for (auto& b : balls)
	{
		// Test for a collision between all the balls and the pocket if true adds that ball to the potted ball vector
		// marks the ball for deletion and plays the potted ball sound effect.
		if (testCollision(v_FromBallToPocket, *b))
		{
			pottedBalls.push_back(*b);
			b->potted();
			Mix_PlayChannel(-1, pocketSoundEffect, 0);
		}
	}
}

void Pocket::setSoundEffects(Mix_Chunk* pocketed)
{
	pocketSoundEffect = pocketed;
}

void Pocket::destroySoundEffects()
{
	Mix_FreeChunk(pocketSoundEffect);
	pocketSoundEffect = NULL;
}