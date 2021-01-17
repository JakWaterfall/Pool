#include "GameEngine.h"

GameEngine::GameEngine()
{
	running = true;
	balls.push_back(Ball(550, 20, 10, 2, false, true));

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

void GameEngine::update()
{
	for (auto& b : balls)
	{
		b.update(balls);
	}
	
}

void GameEngine::eventHandler()
{
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT) running = false;
		for (auto& b : balls)
		{
			b.eventHandler(&e);
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

	// objects render
	for (auto& b : balls)
	{
		b.render(renderer);
	}

	//Update screen
	SDL_RenderPresent(renderer);
}
void GameEngine::renderBackground()
{
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_Rect side = { TABLE_X, TABLE_Y, 200, TABLE_HEIGHT };
	SDL_RenderFillRect(renderer, &side);

	Ball dec = Ball(TABLE_X+200, TABLE_H / 2 + TABLE_Y / 2, 75, 1, true, false);
	dec.render(renderer);

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_Rect cover = { TABLE_X + 200, TABLE_Y, TABLE_WIDTH-200, TABLE_HEIGHT };
	SDL_RenderFillRect(renderer, &cover);

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawLine(renderer, TABLE_X + 200, TABLE_Y, TABLE_X + 200, TABLE_H);

	Ball topLeftPocket = Ball(TABLE_X, TABLE_Y, 20, 1, true, false);
	topLeftPocket.render(renderer);

	Ball topRightPocket = Ball(TABLE_W, TABLE_Y, 20, 1, true, false);
	topRightPocket.render(renderer);

	Ball bottemLeftPocket = Ball(TABLE_X, TABLE_H, 20, 1, true, false);
	bottemLeftPocket.render(renderer);

	Ball bottemRightPocket = Ball(TABLE_W, TABLE_H, 20, 1, true, false);
	bottemRightPocket.render(renderer);

	Ball topMiddlePocket = Ball(TABLE_W/2+TABLE_X/2, TABLE_Y -10, 20, 1, true, false);
	topMiddlePocket.render(renderer);

	Ball bottemMiddlePocket = Ball(TABLE_W / 2 + TABLE_X / 2, TABLE_H + 10, 20, 1, true, false);
	bottemMiddlePocket.render(renderer);

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
