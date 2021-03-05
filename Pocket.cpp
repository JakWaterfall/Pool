#include "Pocket.h"

Mix_Chunk* Pocket::pocketSoundEffect = NULL;

Pocket::Pocket(float _x, float _y): SphereEntity(_x, _y, SphereEntity::Colours::pocket, RADIUS)
{
}

void Pocket::update(std::vector<Ball*>& balls, std::vector<Ball>& pottedBalls)
{
	objectCollision(balls, pottedBalls);
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


void Pocket::objectCollision(std::vector<Ball*>& balls, std::vector<Ball>& pottedBalls)
{
	for (auto& b : balls)
	{
		Vector test = position - b->getPosition();
		float dist = test.magnitude();
		if (dist < radius + b->getRadius() && b->getIsInteractable())  // this is so when the white ball is being dropped it dosent hit the other balls
		{
			pottedBalls.push_back(*b);
			b->potted();
			Mix_PlayChannel(-1, pocketSoundEffect, 0);

			//debug
			std::string colour;

			switch (b->getColour())
			{
			case SphereEntity::Colours::white:
				colour = "white potted";
				break;
			case SphereEntity::Colours::black:
				colour = "black potted";
				break;
			case SphereEntity::Colours::red:
				colour = "red potted";
				break;
			case SphereEntity::Colours::yellow:
				colour = "yellow potted";
				break;
			default:
				break;
			}

			std::cout << "colour: " << colour << std::endl;
		}
	}
}
