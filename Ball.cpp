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
		Vector friction = position - (position + velocity); // Create a vector(friction) pointing from the direction the ball is going to the ball
		friction.setMagnitude(frictionStrength);			// set the magnitude to be a much smaller length so the strength is not too much.
		velocity += friction;								// apply the friction to the velocity to slow the ball down to create the illusion of friction.
		
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
	return velocity.getX() != 0 || velocity.getY() != 0;
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
	Vector v_FromBallToBall(0,0);
	for (auto& b : balls)
	{
		if (b == this) // dont check ball with itself
			continue;

		if (testCollision(v_FromBallToBall, *b))
		{
			v_FromBallToBall.setMagnitude(ballCollisionStrenght); //!!!!!!!! impliment vs speed as well. so calc how fast it was going and use that(poistion - (poistion + velocity))= vector in dir its going then get the magnitude for speed value.
			velocity += v_FromBallToBall; // apply vector to velocity to push ball away from colliding ball in the direction they got hit from.
		}
	}
	
}



void Ball::potted(std::vector<Ball*>& balls)
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
