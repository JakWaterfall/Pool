#include "Ball.h"

Ball::Ball(float _x, float _y, int _radius, bool isRed)
	: position({ _x, _y }), radius(_radius), velocity({ 0, 0 }), mouse({ 0, 0 }), isRed(isRed)
{
	if (isRed)
	{
		r = 0xFF; g = 0x00, b = 0x00;
	}
	else
	{
		r = 0xFF; g = 0xFF, b = 0x00;
	}
}


void Ball::update(std::vector<Ball> & balls)
{
	//velocity += gravity;
	position += velocity;
	//gravToMouse();
	applyFriction();
	wallCollision();
	objectCollision(balls);
}

void Ball::eventHandler(SDL_Event* e)
{
	if (e->type == SDL_MOUSEMOTION)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		mouse.setX(x);
		mouse.setY(y);
		//std::cout << "x: " << mouse.getX() << " y: " << mouse.getY() << std::endl;

	}
	if (e->type == SDL_MOUSEBUTTONDOWN)
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

void Ball::render(SDL_Renderer* renderer)
{
	Vector qwe = position - mouse;
	if (qwe.magnitude() > 100)
	{
		qwe.setMagnitude(100);
	}
	Vector qwe1 = position + qwe;

	SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);
	if (velocity.getX() == 0 && velocity.getY() == 0)SDL_RenderDrawLine(renderer, position.getX(), position.getY(), qwe1.getX(), qwe1.getY());


	drawCirle(renderer, position, radius);
	
}

void Ball::gravToMouse()
{
	Vector move = mouse - position;
	float mag = move.magnitude();
	if (mag <= 40)
	{
		move.setMagnitude(2.0f);
	}
	else if (mag > 40 && mag <= 80)
	{
		move.setMagnitude(1.5f);
	}
	else if (mag > 80 && mag <= 100)
	{
		move.setMagnitude(1.0f);
	}
	else if (mag > 100)
	{
		move.setMagnitude(0.5f);
	}

	velocity += move;
}

void Ball::applyFriction()
{
	if (velocity.getX() != 0)
	{
		if (velocity.getY() != 0)
		{
			Vector friction = position - (position + velocity);
			friction.setMagnitude(0.03);
			velocity += friction;
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

void Ball::objectCollision(std::vector<Ball> & balls)
{
	for (auto& b : balls)
	{
		if (&b != this)
		{
			Vector test = position - b.position;
			float dist = test.magnitude();
			if (dist < radius + b.radius)
			{
				test.setMagnitude(2); // impliment vs speed as well. so calc how fast it was going and use that(poistion - (poistion + velocity))= vector in dir its going then get the magnitude for speed value.
				velocity += test;
			}
		}
	}
}

void Ball::drawCirle(SDL_Renderer* renderer, Vector position, int radius)
{
	// https://www.thecrazyprogrammer.com/2016/12/bresenhams-midpoint-circle-algorithm-c-c.html

	int x0 = position.getX();
	int y0 = position.getY();
	int x = radius;
	int y = 0;
	int err = 0;
	while (x >= y)
	{
		SDL_RenderDrawPoint(renderer, x0 + x, y0 + y);
		SDL_RenderDrawPoint(renderer, x0 + y, y0 + x);
		SDL_RenderDrawPoint(renderer, x0 - y, y0 + x);
		SDL_RenderDrawPoint(renderer, x0 - x, y0 + y);
		SDL_RenderDrawPoint(renderer, x0 - x, y0 - y);
		SDL_RenderDrawPoint(renderer, x0 - y, y0 - x);
		SDL_RenderDrawPoint(renderer, x0 + y, y0 - x);
		SDL_RenderDrawPoint(renderer, x0 + x, y0 - y);

		if (err <= 0)
		{
			y += 1;
			err += 2 * y + 1;
		}

		if (err > 0)
		{
			x -= 1;
			err -= 2 * x + 1;
		}
	}
}