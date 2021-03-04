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


	float frictionStrength = 0.03f; // impliment super slippery mode??
protected:
	float ballCollisionStrenght = 2.0f;
	Vector velocity;
	bool isInteractable = true;
	bool deleteFlag = false;

public:
	Ball(float _x, float _y, SphereEntity::Colours colour, int radius = 10);

public:
	virtual void update(std::vector<Ball*> & balls);
	virtual void potted();
	bool getDeleteFlag();
	bool getIsInteractable() { return isInteractable; }
	Vector& getVelocity();
	static void setHitSoundEffect(Mix_Chunk* sound);
	static void destroyHitSoundEffect();

protected:
	void applyFriction();
	void wallCollision();
	bool ballMoving();
	virtual void ballCollision(std::vector<Ball*> & balls);
	virtual bool testCollision(Vector& v_FromBallToBall, Ball& b);

};

