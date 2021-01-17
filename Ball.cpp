#include "Ball.h"

Ball::Ball(float _x, float _y, int _radius, float mass)
	: position({ _x, _y }), radius(_radius), mass(mass), velocity({ 4, 1 }), mouse({ 0, 0 }), center({ 0, 0 }), acc({ 1, 1 }), gravity({0, 2})
{
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

	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	if (velocity.getX() == 0 && velocity.getY() == 0)SDL_RenderDrawLine(renderer, position.getX(), position.getY(), qwe1.getX(), qwe1.getY());
	for (auto i = 0; i < 1; i++)
	{
		drawCirle(renderer, position, radius-i);
	}
	
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
	if (x - radius < 0)
	{
		velocity.setX(-velocity.getX());
		position.setX(radius);
	}
	if (x + radius > SCREEN_WIDTH)
	{
		velocity.setX(-velocity.getX());
		position.setX(SCREEN_WIDTH - radius);
	}
	if (y - radius < 0)
	{
		velocity.setY(-velocity.getY());
		position.setY(radius);
	}
	if (y + radius > SCREEN_HEIGHT)
	{
		velocity.setY(-velocity.getY());
		position.setY(SCREEN_HEIGHT - radius);
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
				test.setMagnitude(b.getMass()); // impliment vs speed as well. so calc how fast it was going and use that(poistion - (poistion + velocity))= vector in dir its going then get the magnitude for speed value.
				velocity += test;
			}
			

			/*float x = position.getX() - radius;
			float y = position.getY() - radius;
			float w = position.getX() + radius;
			float h = position.getY() + radius;

			float x1 = b.position.getX() - b.radius;
			float y1 = b.position.getY() - b.radius;
			float w1 = b.position.getX() + b.radius;
			float h1 = b.position.getY() + b.radius;

			if (x < w1 && x > x1 && y <= h1 && h >= y1)
			{
				std::cout << "true" << std::endl;
				velocity.setX(-velocity.getX());
				position.setX(b.position.getX() + radius);
			}
			if (w > x1 && w < w1 && y <= h1 && h >= y1)
			{
				velocity.setX(-velocity.getX());
				position.setX(b.position.getX() - radius);
			}
			if (y < h1)
			{
				velocity.setY(-velocity.getY());
				position.setY(h1);
			}
			if ( h > y1)
			{
				velocity.setY(-velocity.getY());
				position.setY(y1);
			}*/
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
