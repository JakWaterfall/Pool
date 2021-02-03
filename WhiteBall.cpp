#include "WhiteBall.h"

WhiteBall::WhiteBall(float _x, float _y, bool dropBall, SphereEntity::Colours colour)
	: Ball(_x, _y, colour), mouse({ 0, 0 }), dropBall(dropBall), info({false, false, false, SphereEntity::Colours::white})
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
	// debug
	if (e->type == SDL_KEYUP)
	{
		velocity.setX(0);
		velocity.setY(0);
		dropBall = true;
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
		info.hit = true;

		Vector hit = position - mouse;
		hit *= 0.1f;
		if (hit.magnitude() > greatestHitStrength)
		{
			hit.setMagnitude(greatestHitStrength);
		}
		velocity += hit;
	}
}

void WhiteBall::ballCollision(std::vector<Ball*>& balls)
{
	Vector v_FromBallToBall(0, 0);
	for (auto& b : balls)
	{
		if (b == this) // dont check ball with itself
			continue;

		if (testCollision(v_FromBallToBall, *b))
		{
			v_FromBallToBall.setMagnitude(ballCollisionStrenght); //!!!!!!!! impliment vs speed as well. so calc how fast it was going and use that(poistion - (poistion + velocity))= vector in dir its going then get the magnitude for speed value.
			velocity += v_FromBallToBall; // apply vector to velocity to push ball away from colliding ball in the direction they got hit from.

			if (!info.hitOtherBall)
			{
				info.hitOtherBall = true;
				info.colourHitFirst = b->getColour();

				std::string colour;
				switch (b->getColour())
				{
				case SphereEntity::Colours::white:
					colour = "white";
					break;
				case SphereEntity::Colours::black:
					colour = "black";
					break;
				case SphereEntity::Colours::red:
					colour = "red";
					break;
				case SphereEntity::Colours::yellow:
					colour = "yellow";
					break;
				default:
					break;
				}
				std::cout << "first ball hit " << colour << std::endl << std::endl;
			}
		}
	}

}

bool WhiteBall::checkIfballsMoving(std::vector<Ball*>& balls)
{
	for (auto& b : balls)
	{
		if (b->getVelocity().getX() != 0 || b->getVelocity().getY() != 0)
			return true;
	}
	if (info.hit)
		info.endTurn = true;
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

		Vector v_FromBallToBall(0, 0);
		if (Ball::testCollision(v_FromBallToBall, *b))
			return true;
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
	/*balls.push_back(new WhiteBall(0, 0, true));
	deleteFlag = true;*/

	velocity.setX(0);
	velocity.setY(0);
	dropBall = true;
	anyBallsMoving = false;
}

