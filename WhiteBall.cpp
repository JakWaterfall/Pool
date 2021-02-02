#include "WhiteBall.h"

WhiteBall::WhiteBall(float _x, float _y, bool dropBall, SphereEntity::Colours colour)
	: Ball(_x, _y, colour), mouse({ 0, 0 }), dropBall(dropBall)
{
}

void WhiteBall::update(std::vector<Ball*>& balls)
{
	anyBallsMoving = checkIfballsMoving(balls);
	if (dropBall)
	{
		radius = 15;
		isInteractable = false;
		collideWithBall = willCollideWithBall(balls);
		keepInDropBallArea();
		return;
	}
	Ball::update(balls);
}


void WhiteBall::render(SDL_Renderer* renderer)
{
	Vector target = position - mouse;		// creates a vector which points from the mouse to the white ball.

	if (target.magnitude() > aimerMaxLenght)
		target.setMagnitude(aimerMaxLenght); // clamp vector magnitude so the aimer doesn't get too big.
	
	Vector aimer = position + target; // adds the vector to the white balls position; transforming the tail of the vector from the mouse to the white ball.

	SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);
	if (!ballMoving() && !dropBall)
		SDL_RenderDrawLine(renderer, position.getX(), position.getY(), aimer.getX(), aimer.getY());

	drawCirle(renderer, position, radius);
}

void WhiteBall::eventHandler(SDL_Event* e)
{
	if (e->type == SDL_KEYUP)
	{
		velocity.setX(0);
		velocity.setY(0);
		dropBall = true;
		//anyBallsMoving = false;
	}

	if (e->type == SDL_MOUSEMOTION)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		mouse.setX(x);
		mouse.setY(y);
	}
	if (dropBall)
	{
		droppingBall(e);
		return;
	}
	if (e->type == SDL_MOUSEBUTTONDOWN && !anyBallsMoving) 
	{
		Vector hit = position - mouse;
		hit *= 0.1f;
		if (hit.magnitude() > greatestHitStrength)
		{
			hit.setMagnitude(greatestHitStrength);
		}
		velocity += hit;
	}
}

bool WhiteBall::checkIfballsMoving(std::vector<Ball*>& balls)
{
	for (auto& b : balls)
	{
		if (b->getVelocity().getX() != 0 || b->getVelocity().getY() != 0)
			return true;
	}
	return false;
}

void WhiteBall::droppingBall(SDL_Event* e)
{
	if (mouseWithinDropBallArea())
	{
		position = mouse;
		if (!collideWithBall && !anyBallsMoving)
		{
			if (e->type == SDL_MOUSEBUTTONDOWN)
			{
				radius = 10;
				isInteractable = true;
				dropBall = false;
			}
		}
	}
}

bool WhiteBall::mouseWithinDropBallArea()
{
	return	mouse.getX() < TABLE_X + TABLE_LINE_FROM_X &&
			mouse.getX() > TABLE_X &&
			mouse.getY() < TABLE_H &&
			mouse.getY() > TABLE_Y;
}

bool WhiteBall::willCollideWithBall(std::vector<Ball*>& balls)
{
	for (auto& b : balls)
	{
		if (b == this)
			continue;

		Vector v_FromBallToBall = position - b->getPosition();	// Create a vector which points from one ball to another.
		float dist = v_FromBallToBall.magnitude();				// return the magnitude of that vector to work out the distance between them.

		// Test to see if the distance between the 2 balls is greater then thier radii, which would indicate the balls were colliding.
		if (dist < radius + b->getRadius())
		{
			return true;
		}
	}
	return false;
}

void WhiteBall::keepInDropBallArea()
{
	int x = position.getX();
	int y = position.getY();

	if (x - radius < TABLE_X)
	{
		position.setX(TABLE_X + radius);
	}
	if (x + radius > TABLE_X + TABLE_LINE_FROM_X)
	{
		position.setX(TABLE_X + TABLE_LINE_FROM_X - radius);
	}
	if (y - radius < TABLE_Y)
	{
		position.setY(TABLE_Y + radius);
	}
	if (y + radius > TABLE_H)
	{
		position.setY(TABLE_H - radius);
	}
}

void WhiteBall::potted(std::vector<Ball*>& balls)
{
	//balls.push_back(new WhiteBall(0, 0, true));
	//deleteFlag = true;
	velocity.setX(0);
	velocity.setY(0);
	dropBall = true;
	anyBallsMoving = false;
}

