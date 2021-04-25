#include "Ball.h"

Mix_Chunk* Ball::hitSoundEffect = NULL;
Mix_Chunk* Ball::hitWallSoundEffect = NULL;

Ball::Ball(float _x, float _y, SphereEntity::Colours colour, int radius)
	: SphereEntity(_x, _y, colour, radius), velocity({ 0.0f, 0.0f })
{
}

/// Updates the position of the ball based on the velocity.
/// Applies friction and calculates wall and ball collisions.
void Ball::update(std::vector<Ball*>& balls)
{
	position += velocity;
	applyFriction();
	wallCollision();
	ballCollision(balls);
}

/// Applies friction to the ball back creating a vector which points backwards to the ball movement direction
/// and sets its magnitude to a constant strength and adds it to the velocity thereby slowing the ball down.
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

/// Tests For a collision with the wall and inverts the x or y velocity based on which wall was hit.
void Ball::wallCollision()
{
	int x = (int)position.getX();
	int y = (int)position.getY();

	if (x - radius < TABLE_X)
	{
		velocity.setX(-velocity.getX()); // Invert velocity away from wall
		position.setX((float)TABLE_X + radius); // set position to be just touching the wall so that the ball may never protrude past the wall
		Mix_PlayChannel(-1, hitWallSoundEffect, 0);
	}
	if (x + radius > TABLE_X2)
	{
		velocity.setX(-velocity.getX());
		position.setX((float)TABLE_X2 - radius);
		Mix_PlayChannel(-1, hitWallSoundEffect, 0);
	}
	if (y - radius < TABLE_Y)
	{
		velocity.setY(-velocity.getY());
		position.setY((float)TABLE_Y + radius);
		Mix_PlayChannel(-1, hitWallSoundEffect, 0);
	}
	if (y + radius > TABLE_Y2)
	{
		velocity.setY(-velocity.getY());
		position.setY((float)TABLE_Y2 - radius);
		Mix_PlayChannel(-1, hitWallSoundEffect, 0);
	}
}

/// Returns a boolean on whether or not he ball is moving.
bool Ball::ballMoving()
{
	return velocity.getX() != 0.0f || velocity.getY() != 0.0f; // if there is a velocity in either the x or y direction the wall is moving.
}

///	Calculates and implements the trajectories of the ball after colliding with another ball.
void Ball::ballCollision(std::vector<Ball*>& balls)
{
	Vector v_FromBallToBall;
	for (auto& b : balls)
	{
		if (b == this) // Don't check ball with itself
			continue;

		if (testCollision(v_FromBallToBall, *b))
		{
			// Vector passed to testCollision function will now hold a vector that is pointing from collided ball and this ball
			// therefore the vector is pointing in the direction in which this ball must be pushed after colliding.
			v_FromBallToBall.setMagnitude(ballCollisionStrenght); // If collided set the magnitude to a constant strength.
			velocity += v_FromBallToBall; // Apply vector to velocity to push ball away from colliding ball in the direction it got hit from.

			// Play hit sound
			auto now = std::chrono::steady_clock::now();
			delta = now - lastTime;
			if (delta.count() > halfSecond) // Sound only plays if it had not played in the last half a second as there are many collisions a second.
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

void Ball::setSoundEffects(Mix_Chunk* hitBall, Mix_Chunk* hitWall)
{
	hitSoundEffect = hitBall;
	hitWallSoundEffect = hitWall;
}

void Ball::destroySoundEffects()
{
	Mix_FreeChunk(hitSoundEffect);
	hitSoundEffect = NULL;
	Mix_FreeChunk(hitWallSoundEffect);
	hitWallSoundEffect = NULL;
}