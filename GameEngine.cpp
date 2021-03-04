#include "GameEngine.h"

GameEngine::GameEngine(bool resume) : whiteBall(0, 0, true), running(true), players(NULL)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Init didnt work! " << SDL_GetError() << std::endl;
	}
	else
	{
		win = SDL_CreateWindow("Jak Waterfall's Billiards!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (!win)
		{
			std::cout << "Creating window didnt work! " << SDL_GetError() << std::endl;
		}
		else
		{
			renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
			if (!renderer)
			{
				std::cout << "Creating renderer didnt work! " << SDL_GetError() << std::endl;
			}
			if (TTF_Init() == -1)
			{
				std::cout << "Init TTF didnt work! " << TTF_GetError() << std::endl;
			}
			if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
			{
				std::cout << "Init SDL_mixer didnt work! " << Mix_GetError() << std::endl;
			}

			running = true;
			players = new Players(); // change this to local if I init font from engine and pass it via parameter.

			// Load Sphere Textures
			SphereEntity::setTextures(
				loadTexture("Images/blackball.bmp"),
				loadTexture("Images/whiteball.bmp"),
				loadTexture("Images/redball.bmp"),
				loadTexture("Images/yellowball.bmp"),
				loadTexture("Images/pocket.bmp")
			);

			// Load sound effects
			Ball::setHitSoundEffect(Mix_LoadWAV("sounds/juskiddink__billiard-balls-single-hit-dry.wav"));

			// Place Balls
			whiteBall = WhiteBall(0, 0, true);
			if (resume)
			{
				setupBallsAndPlayersFromFile();
			}
			else
			{
				placeNewBalls();
			}


			// Pockets
			pockets.push_back(Pocket(TABLE_X, TABLE_Y));
			pockets.push_back(Pocket(TABLE_W, TABLE_Y));
			pockets.push_back(Pocket(TABLE_X, TABLE_H));
			pockets.push_back(Pocket(TABLE_W, TABLE_H));
			pockets.push_back(Pocket(TABLE_W / 2 + TABLE_X / 2, TABLE_Y - 10));
			pockets.push_back(Pocket(TABLE_W / 2 + TABLE_X / 2, TABLE_H + 10));


			run();
		}
	}
}


void GameEngine::run()
{
	// 60 Ticks per second by calling update function every 0.01633 seconds
	std::chrono::duration<float> delta;

	auto lastTime = std::chrono::steady_clock::now();
	float sixtyFramesPerSecond = 0.01633f;

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

	quit();
}

void GameEngine::quit()
{
	if (!players->getGameOver())
	{
		saveGameDialog();
	}

	// DELETE ALL BALL OBJECTS!!!!!!
	for (auto& b : balls)
	{
		if (b != &whiteBall) // because white ball is a local object that does not need deleteing 
			delete b;
	}
	balls.clear();

	delete players;

	SphereEntity::DestroyTextures();
	Ball::destroyHitSoundEffect();

	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	SDL_DestroyWindow(win);
	win = NULL;

	TTF_Quit();
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

void GameEngine::eventHandler()
{
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT) 
			running = false;

		for (auto& b : balls)
		{
			b->eventHandler(&e); // maybe change to only white ball
		}
	}
}

void GameEngine::update()
{

	for (auto& b : balls)
	{
		b->update(balls);
	}

	for (auto& pocket : pockets)
	{
		pocket.update(balls, pottedBalls);
	}
	
	players->update(whiteBall, pottedBalls, balls);

	// Delete Marked Balls
	deleteBalls();
}

void GameEngine::render()
{
	//Clear screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	// draw table
	renderBackground();

	// objects 
	for (auto& pocket : pockets)
	{
		pocket.render(renderer);
	}

	for (auto& b : balls)
	{
		b->render(renderer);
	}
	players->render(renderer);

	//Update screen
	SDL_RenderPresent(renderer);
}

// change variable names 
void GameEngine::renderBackground()
{

	// CHANGE THIS !!
	SDL_SetRenderDrawColor(renderer, 0xB0, 0xB0, 0xB0, 0xFF);
	SDL_Rect background = { 0, 0, SCREEN_WIDTH,  SCREEN_HEIGHT};
	SDL_RenderFillRect(renderer, &background);

	SDL_SetRenderDrawColor(renderer, 0x00, 0xD2, 0x00, 0xFF);
	SDL_Rect side = { TABLE_X, TABLE_Y, TABLE_LINE_FROM_X, TABLE_HEIGHT };
	SDL_RenderFillRect(renderer, &side);

	Ball dec = Ball(TABLE_X + TABLE_LINE_FROM_X, TABLE_H / 2 + TABLE_Y / 2, SphereEntity::Colours::pocket, 75);
	dec.renderWireframe(renderer);

	SDL_SetRenderDrawColor(renderer, 0x00, 0xD2, 0x00, 0xFF);
	SDL_Rect cover = { TABLE_X + TABLE_LINE_FROM_X, TABLE_Y, TABLE_WIDTH - TABLE_LINE_FROM_X, TABLE_HEIGHT };
	SDL_RenderFillRect(renderer, &cover);

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawLine(renderer, TABLE_X + TABLE_LINE_FROM_X, TABLE_Y, TABLE_X + TABLE_LINE_FROM_X, TABLE_H);

	
	dec = Ball(BLACK_POINT_X, BLACK_POINT_Y, SphereEntity::Colours::black, 2);
	dec.render(renderer);
	SDL_RenderDrawPoint(renderer, TABLE_W / 2 + TABLE_X / 2, TABLE_H / 2 + TABLE_Y / 2);

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_Rect tableOutline = { TABLE_X, TABLE_Y, TABLE_WIDTH, TABLE_HEIGHT };
	SDL_RenderDrawRect(renderer, &tableOutline);



	int sideWidth = 40;

	SDL_SetRenderDrawColor(renderer, 0x70, 0x51, 0x2E, 0xFF);
	SDL_Rect tableLeft = { TABLE_X- sideWidth, TABLE_Y- sideWidth, sideWidth, TABLE_HEIGHT+ sideWidth*2 };
	SDL_RenderFillRect(renderer, &tableLeft);

	SDL_SetRenderDrawColor(renderer, 0x70, 0x51, 0x2E, 0xFF);
	SDL_Rect tableTop = { TABLE_X - sideWidth, TABLE_Y - sideWidth, TABLE_WIDTH+ sideWidth*2, sideWidth };
	SDL_RenderFillRect(renderer, &tableTop);

	SDL_SetRenderDrawColor(renderer, 0x70, 0x51, 0x2E, 0xFF);
	SDL_Rect tableRight = { TABLE_X + TABLE_WIDTH, TABLE_Y - sideWidth, sideWidth, TABLE_HEIGHT + sideWidth*2 };
	SDL_RenderFillRect(renderer, &tableRight);

	SDL_SetRenderDrawColor(renderer, 0x70, 0x51, 0x2E, 0xFF);
	SDL_Rect tableBottom = { TABLE_X - sideWidth, TABLE_Y + TABLE_HEIGHT, TABLE_WIDTH + sideWidth * 2, sideWidth };
	SDL_RenderFillRect(renderer, &tableBottom);

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_Rect sideOutline = { TABLE_X - sideWidth, TABLE_Y - sideWidth, TABLE_WIDTH + sideWidth * 2, TABLE_HEIGHT + sideWidth * 2 };
	SDL_RenderDrawRect(renderer, &sideOutline);
}

void GameEngine::placeNewBalls()
{
	balls.push_back(new Ball(BLACK_POINT_X - 40, BLACK_POINT_Y, SphereEntity::Colours::red));
	balls.push_back(new Ball(BLACK_POINT_X, BLACK_POINT_Y, SphereEntity::Colours::black));

	balls.push_back(new Ball(BLACK_POINT_X - 20, BLACK_POINT_Y - 10, SphereEntity::Colours::yellow));
	balls.push_back(new Ball(BLACK_POINT_X - 20, BLACK_POINT_Y + 10, SphereEntity::Colours::yellow));

	balls.push_back(new Ball(BLACK_POINT_X, BLACK_POINT_Y - 20, SphereEntity::Colours::red));
	balls.push_back(new Ball(BLACK_POINT_X, BLACK_POINT_Y + 20, SphereEntity::Colours::red));

	balls.push_back(new Ball(BLACK_POINT_X + 20, BLACK_POINT_Y - 10, SphereEntity::Colours::yellow));
	balls.push_back(new Ball(BLACK_POINT_X + 20, BLACK_POINT_Y + 10, SphereEntity::Colours::red));

	balls.push_back(new Ball(BLACK_POINT_X + 20, BLACK_POINT_Y - 30, SphereEntity::Colours::yellow));
	balls.push_back(new Ball(BLACK_POINT_X + 20, BLACK_POINT_Y + 30, SphereEntity::Colours::yellow));

	balls.push_back(new Ball(BLACK_POINT_X + 40, BLACK_POINT_Y, SphereEntity::Colours::red));

	balls.push_back(new Ball(BLACK_POINT_X + 40, BLACK_POINT_Y - 20, SphereEntity::Colours::yellow));
	balls.push_back(new Ball(BLACK_POINT_X + 40, BLACK_POINT_Y + 20, SphereEntity::Colours::yellow));

	balls.push_back(new Ball(BLACK_POINT_X + 40, BLACK_POINT_Y - 40, SphereEntity::Colours::red));
	balls.push_back(new Ball(BLACK_POINT_X + 40, BLACK_POINT_Y + 40, SphereEntity::Colours::red));

	balls.push_back(&whiteBall);
}

void GameEngine::saveStateOfTable()
{
	using namespace std;

	ofstream ballFile("balls.txt");
	ofstream playerFile("player.txt");

	for (auto& b : balls)
	{
		ballFile << b->getPosition().getX() << endl;
		ballFile << b->getPosition().getY() << endl;
		ballFile << (int)b->getColour() << endl;
	}
	
	Players::saveVariables saveVar = players->getSaveVariables();
	playerFile << saveVar.isPlayer1Turn << endl;
	playerFile << saveVar.arePlayerColoursSetup << endl;
	playerFile << saveVar.player1.ShotsLeft << endl;
	playerFile << saveVar.player2.ShotsLeft << endl;
	playerFile << (int)saveVar.player1.Colour << endl;
	playerFile << (int)saveVar.player2.Colour << endl;

	ballFile.close();
	playerFile.close();
}

void GameEngine::setupBallsAndPlayersFromFile()
{
	using namespace std;

	ifstream ballFile("balls.txt");
	balls.clear();

	float x;
	float y;
	int colour;

	while (true)
	{
		ballFile >> x;
		ballFile >> y;
		ballFile >> colour;
		if (ballFile.eof()) break;
		if ((SphereEntity::Colours)colour == SphereEntity::Colours::white)
		{
			whiteBall = WhiteBall(x, y, false);
			balls.push_back(&whiteBall);
		}
		else
		{
			balls.push_back(new Ball(x, y, (SphereEntity::Colours)colour));
		}
	}
	ballFile.close();

	ifstream playerFile("player.txt");

	Players::saveVariables saveVar;
	playerFile >> saveVar.isPlayer1Turn;
	playerFile >> saveVar.arePlayerColoursSetup;
	playerFile >> saveVar.player1.ShotsLeft;
	playerFile >> saveVar.player2.ShotsLeft;
	int colour1;
	int colour2;
	playerFile >> colour1;
	playerFile >> colour2;
	saveVar.player1.Colour = (SphereEntity::Colours)colour1;
	saveVar.player2.Colour = (SphereEntity::Colours)colour2;

	players->setVariablesFromFile(saveVar);

	playerFile.close();
}

void GameEngine::saveGameDialog()
{
	HWND hwnd = GetConsoleWindow();
	SetForegroundWindow(hwnd); // Brings the console window the the front to allow save game functionality

	while (true)
	{
		char answer;
		std::cout << "Do You Wish To Save The Game? y or n" << std::endl;
		std::cin >> answer;
		switch (answer)
		{
		case 'y':
			saveStateOfTable();
			return;
		case 'n':
			return;
		default:
			std::cout << "Incorrect answer chosen. Place type 'y' or 'n' " << std::endl;
			break;
		}
	}
}

// MAYBE USE THIS FOR BALL IMGS 
SDL_Surface* GameEngine::loadImage(const char* filePath)
{
	SDL_Surface* surface = SDL_LoadBMP(filePath);
	if (!surface)
	{
		std::cout << "load image didnt work! " << SDL_GetError() << std::endl;
	}
	return surface;
}

SDL_Texture* GameEngine::loadTexture(const char* filePath)
{
	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = loadImage(filePath);

	if (loadedSurface == NULL)
	{
		std::cout << "load image didnt work! " << SDL_GetError() << std::endl;
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL)
		{
			std::cout << "load texture didnt work! " << SDL_GetError() << std::endl;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}