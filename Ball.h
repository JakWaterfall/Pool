#pragma once
#include "SphereEntity.h"
#include <chrono>
#include <SDL_mixer.h>

class Ball : public SphereEntity
{
private:
	// Time Variables
	std::chrono::duration<float> delta;
	std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
	float halfSecond = 0.5f;

	static Mix_Chunk* hitSoundEffect;
	static Mix_Chunk* hitWallSoundEffect;

protected:
	Vector velocity;
	float frictionStrength = 0.03f; // implement super slippery mode??
	float ballCollisionStrenght = 2.0f;
	bool deleteFlag = false;

public:
	// Constructor
	Ball(float _x, float _y, SphereEntity::Colours colour, int radius = 10);

public:
	// Functions
	virtual void update(std::vector<Ball*>& balls);
	virtual void potted();
	bool ballMoving();
	bool getDeleteFlag();
	Vector& getVelocity();
	static void setSoundEffects(Mix_Chunk* hitBall, Mix_Chunk* hitWall);
	static void destroySoundEffects();

protected:
	void applyFriction();
	void wallCollision();
	virtual void ballCollision(std::vector<Ball*>& balls);
};
