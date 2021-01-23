#include "Ball.h"

Ball::Ball(float _x, float _y, SphereEntity::Colours colour, int radius)
	: SphereEntity(_x, _y, colour, radius), velocity({ 0, 0 })
{
}


void Ball::update(std::vector<Ball*> & balls)
{
	position += velocity;
	applyFriction();
	wallCollision();
	ballCollision(balls);
}

// what do to with event handler??
void Ball::eventHandler(SDL_Event* e)
{

}


void Ball::applyFriction()
{
	if (ballMoving())
	{
		Vector friction = position - (position + velocity);
		friction.setMagnitude(frictionStrength);
		velocity += friction;
		// clamp velocity so when its close enough to 0; it sets x and y to 0
		if (velocity.getX() < 0.5 && velocity.getX() > -0.5)
		{
			if (velocity.getY() < 0.5 && velocity.getY() > -0.5)
			{
				velocity.setX(0);
				velocity.setY(0);
			}
		}
	}
}

void Ball::wallCollision()
{
	int x = position.getX();
	int y = position.getY();
	if (x - radius < TABLE_X)
	{
		velocity.setX(-velocity.getX());
		position.setX(TABLE_X+radius);
	}
	if (x + radius > TABLE_W)
	{
		velocity.setX(-velocity.getX());
		position.setX(TABLE_W - radius);
	}
	if (y - radius < TABLE_Y)
	{
		velocity.setY(-velocity.getY());
		position.setY(TABLE_Y+radius);
	}
	if (y + radius > TABLE_H)
	{
		velocity.setY(-velocity.getY());
		position.setY(TABLE_H - radius);
	}
}

bool Ball::ballMoving()
{
	return velocity.getX() != 0 && velocity.getY() != 0;
}

// maybe split up the collision test for testing on ball drop???
void Ball::ballCollision(std::vector<Ball*> & balls)
{
	for (auto& b : balls)
	{
		if (b != this)
		{
			Vector v_FromBallToBall = position - b->position; // change variables names from test
			float dist = v_FromBallToBall.magnitude();
			if (dist < radius + b->radius && b->isInteractable) // this is so when the white ball is being dropped it dosent hit the other balls
			{
				v_FromBallToBall.setMagnitude(2); // impliment vs speed as well. so calc how fast it was going and use that(poistion - (poistion + velocity))= vector in dir its going then get the magnitude for speed value.
				velocity += v_FromBallToBall;
			}
		}
	}
}


void Ball::markForDelete(std::vector<Ball*>& balls)
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
