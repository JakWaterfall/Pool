#include "WhiteBall.h"

WhiteBall::WhiteBall(float _x, float _y, bool dropBall, SphereEntity::Colours colour)
	: Ball(_x, _y, colour), mouse({ 0, 0 }), dropBall(dropBall)
{
}

void WhiteBall::update(std::vector<Ball*>& balls)
{
	if (dropBall)
	{
		radius = 15;
		isInteractable = false;
		dropBallCollision();
		return;
	}
	Ball::update(balls);
}


void WhiteBall::render(SDL_Renderer* renderer)
{
	Vector target = position - mouse;
	if (target.magnitude() > aimerMaxLenght)
	{
		target.setMagnitude(aimerMaxLenght);
	}
	Vector aimer = position + target;

	SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);
	if (velocity.getX() == 0 && velocity.getY() == 0 && !dropBall)
		SDL_RenderDrawLine(renderer, position.getX(), position.getY(), aimer.getX(), aimer.getY());

	drawCirle(renderer, position, radius);
}

void WhiteBall::eventHandler(SDL_Event* e)
{
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
	if (e->type == SDL_MOUSEBUTTONDOWN && velocity.getX() == 0 && velocity.getY() == 0) // change to only hit if all balls vel is 0
	{
		Vector hit = position - mouse;
		hit *= 0.1;
		if (hit.magnitude() > 10)
		{
			hit.setMagnitude(10);
		}
		velocity += hit;
	}
}

void WhiteBall::droppingBall(SDL_Event* e)
{
	// Check mouse is in ball drop area
	if (mouse.getX() < TABLE_X + TABLE_LINE_FROM_X && mouse.getX() > TABLE_X)
	{
		if (mouse.getY() < TABLE_H && mouse.getY() > TABLE_Y)
		{
			position = mouse;
			if (e->type == SDL_MOUSEBUTTONDOWN)
			{
				radius = 10;
				isInteractable = true;
				dropBall = false;
			}
		}
	}
}

void WhiteBall::dropBallCollision()
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

void WhiteBall::markForDelete(std::vector<Ball*>& balls)
{
	balls.push_back(new WhiteBall(0, 0, true));
	deleteFlag = true;
}

