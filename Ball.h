#pragma once
#include "SphereEntity.h"
#include <chrono>
#include <SDL_mixer.h>

class Ball : public SphereEntity
{	
	std::chrono::duration<float> delta;
	std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
	float halfSecond = 0.5f;
	static Mix_Chunk* hitSoundEffect;
	static Mix_Chunk* hitWallSoundEffect;


	float frictionStrength = 0.03f; // impliment super slippery mode??
protected:
	float ballCollisionStrenght = 2.0f;
	Vector velocity;
	bool deleteFlag = false;

public:
	Ball(float _x, float _y, SphereEntity::Colours colour, int radius = 10);

public:
	virtual void update(std::vector<Ball*> & balls);
	virtual void potted();
	bool getDeleteFlag();
	bool ballMoving();
	Vector& getVelocity();
	static void setSoundEffects(Mix_Chunk* hitBall, Mix_Chunk* hitWall);
	static void destroySoundEffects();

protected:
	void applyFriction();
	void wallCollision();
	virtual void ballCollision(std::vector<Ball*> & balls);
	bool testCollision(Vector& v_FromBallToBall, SphereEntity& object);
};
