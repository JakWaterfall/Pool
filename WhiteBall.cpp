#include "WhiteBall.h"

WhiteBall::WhiteBall(float _x, float _y, bool dropBall, std::vector<Pocket>& pockets, SphereEntity::Colours colour)
	: Ball(_x, _y, colour), mouse({ 0, 0 }), dropBall(dropBall), pockets(pockets), info({ false, false, false, SphereEntity::Colours::white })
{
}

///	Checks if any balls on the table are moving.
/// If currently in drop ball state, it forces the white ball to be kept in the drop ball area,
/// enlargers the radius to give the illusion that the ball is being held above the table and finally
/// checks if the white ball will collide with all balls or pockets so that the ball cannot be dropped onto the other balls or into a pocket.
/// The white ball then updates its position and applies friction as per the Ball::Update function.
void WhiteBall::update(std::vector<Ball*>& balls)
{
	anyBallsMoving = checkIfballsMoving(balls);
	if (dropBall)
	{
		radius = 15;
		isInteractable = false; // if the white ball is being held above the table is must not interact with other balls and pockets
		collideWithBall = willCollideWithBall(balls);
		collideWithPocket = willCollideWithPocket();
		keepInDropBallArea();
		return;
	}
	Ball::update(balls);
}

///	Renders the white ball and renders a aimer line from the ball and the direction the player is aiming the hit the ball to screen.
void WhiteBall::render(SDL_Renderer* renderer)
{
	Vector v_fromMouseToBall = position - mouse;		// Creates a vector which points from the mouse to the white ball.

	if (v_fromMouseToBall.magnitude() > aimerMaxLenght)
		v_fromMouseToBall.setMagnitude(aimerMaxLenght); // Clamp vector magnitude so the aimer doesn't get too big.

	// Add the vector to the white balls position; transforming the vector so that the head of the vector starts that the white ball and points away from the mouse.
	Vector aimer = position + v_fromMouseToBall;

	if (!ballMoving() && !dropBall)
		SDL_RenderDrawLine(renderer, (int)position.getX(), (int)position.getY(), (int)aimer.getX(), (int)aimer.getY());

	SphereEntity::render(renderer);
}

///	Handles events such as clicking the mouse for hitting the white ball or dropping the white ball.
void WhiteBall::eventHandler(SDL_Event* e)
{
	// Debug press any key to make the white ball into dropping mode.
	if (e->type == SDL_KEYUP)
	{
		velocity.setX(0);
		velocity.setY(0);
		dropBall = true;
	}
	if (e->type == SDL_MOUSEMOTION)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);				// Get state of mouse for manipulating the white ball (hit it, drop it).
		mouse.setX((float)x);
		mouse.setY((float)y);
	}
	if (dropBall)
	{
		droppingBall(e);
		return;									// if dropping ball mode return so that the player can not hit the ball.
	}
	if (e->type == SDL_MOUSEBUTTONDOWN && !anyBallsMoving)
	{
		info.hit = true;						// Marks as hitting the white ball.

		Vector hit = position - mouse;			// Create vector which points from the mouse to the white ball.
		hit *= 0.1f;							// Remove 90% of the magnitude of the vector to apply a smaller force.
		if (hit.magnitude() > maxHitStrength)	// Clamp hit strength to reasonable force for game play.
		{
			hit.setMagnitude(maxHitStrength);
		}
		velocity += hit;						// Add hit vector to velocity to move ball in that direction and speed.
	}
}

/// Calculates and implements the trajectories of the ball after colliding with another ball.
void WhiteBall::ballCollision(std::vector<Ball*>& balls)
{
	Vector v_FromBallToBall;
	for (auto& b : balls)
	{
		if (b == this) // Don't check ball with itself.
			continue;

		if (testCollision(v_FromBallToBall, *b))
		{
			// Vector passed to testCollision function will now hold a vector that is pointing from collided ball and this ball
			// therefore the vector is pointing in the direction in which this ball must be pushed after colliding.
			v_FromBallToBall.setMagnitude(ballCollisionStrenght); // if collided set the magnitude to a constant strength.
			velocity += v_FromBallToBall; // Apply vector to velocity to push ball away from colliding ball in the direction it got hit from.

			if (!info.hitOtherBall) // Only triggers if the white ball had not hit another ball previously.
			{
				info.hitOtherBall = true;				// Marks as hitting another ball after.
				info.colourHitFirst = b->getColour();	// Stores the colour of the first ball it hit.
			}
		}
	}
}

///	Checks every ball on the table if any are currently moving.
bool WhiteBall::checkIfballsMoving(std::vector<Ball*>& balls)
{
	for (auto& b : balls)
	{
		if (b->ballMoving())	// Check if any ball is moving; return true if so.
			return true;
	}
	if (info.hit)
		info.endTurn = true;	// If the white ball had been hit and all the balls have come to a stop then turn is over.
	return false;
}

/// Allows the player to drop the ball from inside the drop ball area (behind the line with the semi-circle).
void WhiteBall::droppingBall(SDL_Event* e)
{
	if (mouseWithinDropBallArea())	// Only allows the user to drop the ball if mouse is within the drop ball area.
	{
		position = mouse; // Places the white ball onto the cursor.
		if (!collideWithBall && !collideWithPocket && !anyBallsMoving) // Cannot drop the ball onto another ball or pocket, or if any other ball is currently moving.
		{
			if (e->type == SDL_MOUSEBUTTONDOWN)
			{
				radius = 10;			// Radius returned to normal to give the illusion the ball is now back on the table.
				isInteractable = true;	// Other balls may now interact with it.
				dropBall = false;
			}
		}
	}
}

///	Returns a boolean of whether or not the mouse is within the drop ball area.
bool WhiteBall::mouseWithinDropBallArea()
{
	return	mouse.getX() < TABLE_X + TABLE_LINE_FROM_X &&
		mouse.getX() > TABLE_X &&
		mouse.getY() < TABLE_Y2 &&
		mouse.getY() > TABLE_Y;
}

///	Tests whether or not the white ball will collide with another ball.
bool WhiteBall::willCollideWithBall(const std::vector<Ball*>& balls)
{
	for (auto& b : balls)
	{
		if (b == this) // Don't test with itself
			continue;

		Vector v_FromBallToBall;
		if (Ball::testCollision(v_FromBallToBall, *b))
			return true;
	}
	return false;
}

/// Tests whether or not the white ball will collide with a pocket.
bool WhiteBall::willCollideWithPocket()
{
	for (auto& pocket : pockets)
	{
		Vector v_FromBallToBall;
		if (Ball::testCollision(v_FromBallToBall, pocket))
			return true;
	}
	return false;
}

/// Stops the white ball from leaving the drop ball area.
/// This function is used when in drop ball mode.
void WhiteBall::keepInDropBallArea()
{
	int x = (int)position.getX();
	int y = (int)position.getY();

	// Checks whether the white ball is out of bounds of the drop ball area and sets its position accordingly if so.
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
}

/// This function is called from the Players object and resets the Info data ready for the next turn.
void WhiteBall::resetInfo()
{
	info.hit = false;
	info.endTurn = false;
	info.hitOtherBall = false;
}