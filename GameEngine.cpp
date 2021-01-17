#include "GameEngine.h"

GameEngine::GameEngine()
{
	running = true;
	// Balls
	balls.push_back(new Ball(BLACK_POINT_X-40, BLACK_POINT_Y, true));
	balls.push_back(new Ball(BLACK_POINT_X, BLACK_POINT_Y, true));

	balls.push_back(new Ball(BLACK_POINT_X-20, BLACK_POINT_Y-10, false));
	balls.push_back(new Ball(BLACK_POINT_X-20, BLACK_POINT_Y+10, false));

	balls.push_back(new Ball(BLACK_POINT_X, BLACK_POINT_Y-20, true));
	balls.push_back(new Ball(BLACK_POINT_X, BLACK_POINT_Y+20, true));
	//
	balls.push_back(new Ball(BLACK_POINT_X + 20, BLACK_POINT_Y - 10, false));
	balls.push_back(new Ball(BLACK_POINT_X + 20, BLACK_POINT_Y + 10, true));

	balls.push_back(new Ball(BLACK_POINT_X + 20, BLACK_POINT_Y - 30, false));
	balls.push_back(new Ball(BLACK_POINT_X + 20, BLACK_POINT_Y + 30, false));

	balls.push_back(new Ball(BLACK_POINT_X + 40, BLACK_POINT_Y, true));

	balls.push_back(new Ball(BLACK_POINT_X + 40, BLACK_POINT_Y-20, false));
	balls.push_back(new Ball(BLACK_POINT_X + 40, BLACK_POINT_Y+20, false));

	balls.push_back(new Ball(BLACK_POINT_X + 40, BLACK_POINT_Y - 40, true));
	balls.push_back(new Ball(BLACK_POINT_X + 40, BLACK_POINT_Y + 40, true));

	balls.push_back(new WhiteBall(0, 0, true));

	// Pockets
	pockets.push_back(Pocket(TABLE_X, TABLE_Y));
	pockets.push_back(Pocket(TABLE_W, TABLE_Y));
	pockets.push_back(Pocket(TABLE_X, TABLE_H));
	pockets.push_back(Pocket(TABLE_W, TABLE_H));
	pockets.push_back(Pocket(TABLE_W / 2 + TABLE_X / 2, TABLE_Y - 10));
	pockets.push_back(Pocket(TABLE_W / 2 + TABLE_X / 2, TABLE_H + 10));


	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Init didnt work! " << SDL_GetError() << std::endl;
	}
	else
	{
		win = SDL_CreateWindow("Testing! :D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (!win)
		{
			std::cout << "Creating window didnt work! " << SDL_GetError() << std::endl;
		}
		else
		{
			winSurface = SDL_GetWindowSurface(win);

			renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
			if (!renderer)
			{
				std::cout << "Creating renderer didnt work! " << SDL_GetError() << std::endl;
			}

			run();
		}
	}
}


void GameEngine::run()
{
	// 60 Ticks per second by calling update function every 0.01633 seconds
	std::chrono::duration<float> delta;
	auto lastTime = std::chrono::steady_clock::now();
	float sixtyFramesPerSecond = 0.01633;
	while (running)
	{
		// Event Queue
		eventHandler();
		auto now = std::chrono::steady_clock::now();
		delta = now - lastTime;
		while (delta.count() > sixtyFramesPerSecond)
		{
		update();
		lastTime = std::chrono::steady_clock::now();
		delta = now - lastTime;
		}

		render();
	}

	// End Game
	quit();
}

void GameEngine::quit()
{

	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	SDL_DestroyWindow(win);
	win = NULL;

	SDL_Quit();
}

void GameEngine::deleteBalls()
{
	balls.erase(std::remove_if(balls.begin(), balls.end(),
		[this](Ball* b)
		{
			if (b->getDeleteFlag())
			{
				delete b;
				b = nullptr;
				return true;
			}
			else return false;
		}
	), balls.end());
}

void GameEngine::update()
{

	for (auto& b : balls)
	{
		b->update(balls);
	}

	for (auto& pocket : pockets)
	{
		pocket.update(balls);
	}
	// Delete Marked Balls
	deleteBalls();
}

void GameEngine::eventHandler()
{
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT) running = false;
		for (auto& b : balls)
		{
			b->eventHandler(&e);
		}
	}
}


void GameEngine::render()
{
	//Clear screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	// draw table
	renderBackground();

	// objects 
	for (auto& b : balls)
	{
		b->render(renderer);
	}

	//Update screen
	SDL_RenderPresent(renderer);
}
void GameEngine::renderBackground()
{
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_Rect side = { TABLE_X, TABLE_Y, TABLE_LINE_FROM_X, TABLE_HEIGHT };
	SDL_RenderFillRect(renderer, &side);

	Ball dec = Ball(TABLE_X + TABLE_LINE_FROM_X, TABLE_H / 2 + TABLE_Y / 2, true, 75);
	dec.render(renderer);

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_Rect cover = { TABLE_X + TABLE_LINE_FROM_X, TABLE_Y, TABLE_WIDTH - TABLE_LINE_FROM_X, TABLE_HEIGHT };
	SDL_RenderFillRect(renderer, &cover);

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawLine(renderer, TABLE_X + TABLE_LINE_FROM_X, TABLE_Y, TABLE_X + TABLE_LINE_FROM_X, TABLE_H);

	
	dec = Ball(BLACK_POINT_X, BLACK_POINT_Y, true, 2);
	dec.render(renderer);
	SDL_RenderDrawPoint(renderer, TABLE_W / 2 + TABLE_X / 2, TABLE_H / 2 + TABLE_Y / 2);

	for (auto& pocket : pockets)
	{
		pocket.render(renderer);
	}

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_Rect tableOutline = { TABLE_X, TABLE_Y, TABLE_WIDTH, TABLE_HEIGHT };
	SDL_RenderDrawRect(renderer, &tableOutline);
}

SDL_Surface* GameEngine::loadImage(const char* filePath)
{
	SDL_Surface* surface = SDL_LoadBMP(filePath);
	if (!surface)
	{
		std::cout << "load image didnt work! " << SDL_GetError() << std::endl;
	}
	return surface;
}
