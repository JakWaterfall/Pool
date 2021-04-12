#include "WhiteBall.h"

WhiteBall::WhiteBall(float _x, float _y, bool dropBall, std::vector<Pocket>& pockets, SphereEntity::Colours colour)
	: Ball(_x, _y, colour), mouse({ 0, 0 }), dropBall(dropBall), pockets(pockets), info({false, false, false, SphereEntity::Colours::white})
{
}

void WhiteBall::update(std::vector<Ball*>& balls)
{
	anyBallsMoving = checkIfballsMoving(balls);
	if (dropBall)
	{
		radius = 15; // maybe remove this and put in potted
		isInteractable = false; // maybe remove this and put in potted
		collideWithBall = willCollideWithBall(balls);
		collideWithPocket = willCollideWithPocket();
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
		SDL_RenderDrawLine(renderer, (int)position.getX(), (int)position.getY(), (int)aimer.getX(), (int)aimer.getY());

	SphereEntity::render(renderer);
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
		mouse.setX((float)x);
		mouse.setY((float)y);
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
		if (hit.magnitude() > maxHitStrength)
		{
			hit.setMagnitude(maxHitStrength);
		}
		velocity += hit;
	}
}

void WhiteBall::ballCollision(std::vector<Ball*>& balls)
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

			if (!info.hitOtherBall)
			{
				info.hitOtherBall = true;
				info.colourHitFirst = b->getColour();

				// debug
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
		if (b->ballMoving())
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
		if (!collideWithBall && !collideWithPocket && !anyBallsMoving)
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
			mouse.getY() < TABLE_Y2 &&
			mouse.getY() > TABLE_Y;
}

bool WhiteBall::willCollideWithBall(std::vector<Ball*>& balls)
{
	for (auto& b : balls)
	{
		if (b == this)
			continue;

		Vector v_FromBallToBall;
		if (Ball::testCollision(v_FromBallToBall, *b))
			return true;
	}
	return false;
}

bool WhiteBall::willCollideWithPocket()
{
	for (auto& pocket : pockets)
	{
		Vector v_FromBallToBall = position - pocket.getPosition();	// Create a vector which points from the whiteball to the pocket
		float dist = v_FromBallToBall.magnitude();			// return the magnitude of that vector to work out the distance between them.

		// Test to see if the distance between the whiteball and the pocket is greater then thier radii, which would indicate the objects were colliding.
		if (dist < radius + pocket.getRadius())
		{
			return true;
		}
	}
		return false;
}

void WhiteBall::keepInDropBallArea()
{
	int x = (int)position.getX();
	int y = (int)position.getY();

	if (x - radius < TABLE_X)
	{
		position.setX((float)TABLE_X + radius);
	}
	if (x + radius > TABLE_X + TABLE_LINE_FROM_X)
	{
		position.setX((float)TABLE_X + TABLE_LINE_FROM_X - radius);
	}
	if (y - radius < TABLE_Y)
	{
		position.setY((float)TABLE_Y + radius);
	}
	if (y + radius > TABLE_Y2)
	{
		position.setY((float)TABLE_Y2 - radius);
	}
}

void WhiteBall::potted()
{
	velocity.setX(0);
	velocity.setY(0);
	dropBall = true;
	anyBallsMoving = false; // dont think this needs to be here? 

	isInteractable = false; // this up update one needs removing?
}

void WhiteBall::resetInfo()
{
	info.hit = false;
	info.endTurn = false;
	info.hitOtherBall = false;
}

