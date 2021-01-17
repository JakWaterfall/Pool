#include "GameEngine.h"

GameEngine::GameEngine()
{
	running = true;
	balls.push_back(Ball(550, 20, 10, 2));

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
