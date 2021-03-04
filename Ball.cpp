#include "Ball.h"

Mix_Chunk* Ball::hitSoundEffect = NULL;

Ball::Ball(float _x, float _y, SphereEntity::Colours colour, int radius)
	: SphereEntity(_x, _y, colour, radius), velocity({ 0.0f, 0.0f })
{
}


void Ball::update(std::vector<Ball*> & balls)
{
	position += velocity;
	applyFriction();
	wallCollision();
	ballCollision(balls);
}


void Ball::applyFriction()
{
	if (ballMoving())
	{
		Vector friction = position - (position + velocity); // Create a vector(friction) pointing from the direction the ball is going to the ball
		friction.setMagnitude(frictionStrength);			// set the magnitude to be a much smaller length so the strength is not too much.
		velocity += friction;								// apply the friction to the velocity to slow the ball down to create the illusion of friction.
		
		// clamp velocity so when its close enough to 0; it sets x and y to 0
		if (velocity.getX() < 0.5f && velocity.getX() > -0.5f)
		{
			if (velocity.getY() < 0.5f && velocity.getY() > -0.5f)
			{
				velocity.setX(0.0f);
				velocity.setY(0.0f);
			}
		}
	}
}

void Ball::wallCollision()
{
	int x = (int)position.getX();
	int y = (int)position.getY();

	if (x - radius < TABLE_X)
	{
		velocity.setX(-velocity.getX());
		position.setX((float)TABLE_X+radius);
	}
	if (x + radius > TABLE_W)
	{
		velocity.setX(-velocity.getX());
		position.setX((float)TABLE_W - radius);
	}
	if (y - radius < TABLE_Y)
	{
		velocity.setY(-velocity.getY());
		position.setY((float)TABLE_Y+radius);
	}
	if (y + radius > TABLE_H)
	{
		velocity.setY(-velocity.getY());
		position.setY((float)TABLE_H - radius);
	}
}

bool Ball::ballMoving()
{
	return velocity.getX() != 0.0f || velocity.getY() != 0.0f;
}


bool Ball::testCollision(Vector & v_FromBallToBall, Ball & b)
{
	v_FromBallToBall = position - b.position;	// Create a vector which points from one ball to another.
	float dist = v_FromBallToBall.magnitude();			// return the magnitude of that vector to work out the distance between them.

	// Test to see if the distance between the 2 balls is greater then thier radii, which would indicate the balls were colliding.
	if (dist < radius + b.radius && b.isInteractable) // isInteractable so when the white ball is being dropped it dosent hit the other balls
	{
		return true;
	}
	return false;
}

void Ball::ballCollision(std::vector<Ball*>& balls)
{
	Vector v_FromBallToBall;
	for (auto& b : balls)
	{
		if (b == this) // dont check ball with itself
			continue;

		if (testCollision(v_FromBallToBall, *b))
		{
			v_FromBallToBall.setMagnitude(ballCollisionStrenght); //!!!!!!!! impliment vs speed as well. so calc how fast it was going and use that(poistion - (poistion + velocity))= vector in dir its going then get the magnitude for speed value.
			velocity += v_FromBallToBall; // apply vector to velocity to push ball away from colliding ball in the direction they got hit from.

			// play hit sound
			auto now = std::chrono::steady_clock::now();
			delta = now - lastTime;
			if (delta.count() > halfSecond)
			{
				Mix_PlayChannel(-1, hitSoundEffect, 0);
				lastTime = std::chrono::steady_clock::now();
			}

		}
	}
	
}



void Ball::potted()
{
	deleteFlag = true;
}

bool Ball::getDeleteFlag()
{
	return deleteFlag;
}

Vector& Ball::getVelocity()
{
	return velocity;
}

void Ball::setHitSoundEffect(Mix_Chunk* sound)
{
	hitSoundEffect = sound;
}

void Ball::destroyHitSoundEffect()
{
	Mix_FreeChunk(hitSoundEffect);
	hitSoundEffect = NULL;
}