#include "GameEngine.h"

///	Constructor initialise the SDL framework and variables to play the game.
GameEngine::GameEngine(bool resume, int pocketSize) : whiteBall(0, 0, true, pockets), running(true), players(NULL)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) // Initialise SDL.
	{
		std::cout << "Init didn't work! " << SDL_GetError() << std::endl;
	}
	else
	{
		// Create SDL Window object.
		win = SDL_CreateWindow("Jak Waterfall's Billiards!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (!win)
		{
			std::cout << "Creating window didn't work! " << SDL_GetError() << std::endl;
		}
		else
		{
			// Create SDL Renderer object for drawing to the screen.
			renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
			if (!renderer)
			{
				std::cout << "Creating renderer didn't work! " << SDL_GetError() << std::endl;
			}
			if (TTF_Init() == -1) // Initialise SDL Text Fonts for drawing test to screen.
			{
				std::cout << "Init TTF didn't work! " << TTF_GetError() << std::endl;
			}
			if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) // Initialise SDL Mixer for playing sound.
			{
				std::cout << "Init SDL_mixer didn't work! " << Mix_GetError() << std::endl;
			}

			// Players
			players = new Players();

			// Load Sphere Textures
			SphereEntity::setTextures(
				loadTexture("Images/blackball.bmp"),
				loadTexture("Images/whiteball40.bmp"),
				loadTexture("Images/redball.bmp"),
				loadTexture("Images/yellowball.bmp"),
				loadTexture("Images/pocket100.bmp"),
				loadTexture("Images/dec2.bmp")
			);

			// Load sound effects
			Ball::setSoundEffects(
				Mix_LoadWAV("sounds/HitBallSound2.wav"),
				Mix_LoadWAV("sounds/HitWallSound2.wav")
			);
			Pocket::setSoundEffects(Mix_LoadWAV("sounds/PottingSound.wav"));

			// Pockets
			pockets.push_back(Pocket(TABLE_X, TABLE_Y, pocketSize));
			pockets.push_back(Pocket(TABLE_X2, TABLE_Y, pocketSize));
			pockets.push_back(Pocket(TABLE_X, TABLE_Y2, pocketSize));
			pockets.push_back(Pocket(TABLE_X2, TABLE_Y2, pocketSize));
			pockets.push_back(Pocket(TABLE_X2 / 2 + TABLE_X / 2, TABLE_Y - 10, pocketSize));
			pockets.push_back(Pocket(TABLE_X2 / 2 + TABLE_X / 2, TABLE_Y2 + 10, pocketSize));

			// Place Balls. If the user choose to resume game the balls are placed from file.
			whiteBall = WhiteBall(0, 0, true, pockets);
			if (resume)
			{
				setupBallsAndPlayersFromFile();
			}
			else
			{
				placeNewBalls();
			}

			// Init Decoration Rect's for drawing the table and background.
			grayBackground = { 0, 0, SCREEN_WIDTH,  SCREEN_HEIGHT };
			dropBallAreaFelt = { TABLE_X, TABLE_Y, TABLE_LINE_FROM_X, TABLE_HEIGHT };
			tableFelt = { TABLE_X + TABLE_LINE_FROM_X, TABLE_Y, TABLE_WIDTH - TABLE_LINE_FROM_X, TABLE_HEIGHT };
			tableOutline = { TABLE_X, TABLE_Y, TABLE_WIDTH, TABLE_HEIGHT };
			constexpr int sideWidth = 40;
			tableWallLeft = { TABLE_X - sideWidth, TABLE_Y - sideWidth, sideWidth, TABLE_HEIGHT + sideWidth * 2 };
			tableWallTop = { TABLE_X - sideWidth, TABLE_Y - sideWidth, TABLE_WIDTH + sideWidth * 2, sideWidth };
			tableWallRight = { TABLE_X + TABLE_WIDTH, TABLE_Y - sideWidth, sideWidth, TABLE_HEIGHT + sideWidth * 2 };
			tableWallBottom = { TABLE_X - sideWidth, TABLE_Y + TABLE_HEIGHT, TABLE_WIDTH + sideWidth * 2, sideWidth };
			tableWallOutline = { TABLE_X - sideWidth, TABLE_Y - sideWidth, TABLE_WIDTH + sideWidth * 2, TABLE_HEIGHT + sideWidth * 2 };

			run(); // Run the Game Loop.
		}
	}
}

/// Runs the game loop. Game loop consists of 3 phases
/// 1st it handles all the events (such as mouse and keyboard events)
/// 2nd it updates all the objects (such as ball positions and collisions)
/// 3rd it renders all the objects to the screen.
///
/// To keep simulation speed equal on all machines the update cycle is based on time.
/// The update function is triggered around 60 times a second.
void GameEngine::run()
{
	// 60 Updates per second by calling update function every 0.01633 seconds.
	std::chrono::duration<float> delta;
	auto lastTime = std::chrono::steady_clock::now();
	float sixtiethOfASecond = 0.01633f;

	while (running) // Game Loop.
	{
		// Event Queue
		eventHandler();

		auto now = std::chrono::steady_clock::now();
		delta = now - lastTime;
		while (delta.count() > sixtiethOfASecond) // update function is called if the time between last it was called and now (the delta) is greater than 1/60 of a second
		{
			update(); // updates all the objects
			lastTime = std::chrono::steady_clock::now(); // update lastTime variable to be straight after function call
			delta = now - lastTime; // compute new delta (difference between now and last time)
		}
		render(); // renders all the objects to screen
	}

	quit(); // Quits the game on exit
}

/// Quits the game after the game loop has finished.
/// If the game is not over it allows the player to save to game to file.
/// Deletes all the objects and pointers.
void GameEngine::quit()
{
	if (!players->getGameOver()) // If game is not over present save game option.
	{
		saveGameDialogue();
	}

	// Delete all the ball, player, texture and sound pointers
	for (auto& b : balls)
	{
		if (b != &whiteBall) // because white ball is a local object it does not need deleting
			delete b;
	}
	balls.clear();

	delete players;

	SphereEntity::destroyTextures();
	Ball::destroySoundEffects();
	Pocket::destroySoundEffects();

	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	SDL_DestroyWindow(win);
	win = NULL;

	TTF_Quit();
	SDL_Quit();
}

/// Handles the mouse and keyboard events.
void GameEngine::eventHandler()
{
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT) // If the close button is pressed the game loop is stopped.
			running = false;

		whiteBall.eventHandler(&e);
	}
}

/// Updates all the objects of the game.
/// Calls delete function on any balls marked from being potted.
void GameEngine::update()
{
	for (auto& b : balls)
	{
		b->update(balls); // Update all the balls
	}

	for (auto& pocket : pockets)
	{
		pocket.update(balls, pottedBalls); // Update all the pockets
	}

	players->update(whiteBall, pottedBalls, balls); // Update the player info

	// Delete Marked Balls
	deleteBalls();
}

/// Render all the objects to screen.
/// Clears the screen then renders the table, background, balls, pockets and player info.
void GameEngine::render()
{
	//Clear screen
	SDL_SetRenderDrawColor(renderer, 0xB0, 0xB0, 0xB0, 0xFF); // Set colour grey
	SDL_RenderClear(renderer);

	// Draw table
	renderBackground();

	// Objects
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

/// Renders the background and table.
void GameEngine::renderBackground()
{
	SDL_SetRenderDrawColor(renderer, 0x00, 0xD2, 0x00, 0xFF); // Set colour green
	SDL_RenderFillRect(renderer, &dropBallAreaFelt);

	Ball halfCircle = Ball(TABLE_X + TABLE_LINE_FROM_X, TABLE_Y + TABLE_HEIGHT / 2, SphereEntity::Colours::decor, 75);
	halfCircle.render(renderer);

	SDL_RenderFillRect(renderer, &tableFelt);

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF); // Set colour black
	SDL_RenderDrawLine(renderer, TABLE_X + TABLE_LINE_FROM_X, TABLE_Y, TABLE_X + TABLE_LINE_FROM_X, TABLE_Y2);

	Ball dot = Ball(BLACK_POINT_X, BLACK_POINT_Y, SphereEntity::Colours::black, 2);
	dot.render(renderer);

	SDL_RenderDrawRect(renderer, &tableOutline);

	SDL_SetRenderDrawColor(renderer, 0x70, 0x51, 0x2E, 0xFF); // Set colour brown
	SDL_RenderFillRect(renderer, &tableWallLeft);
	SDL_RenderFillRect(renderer, &tableWallTop);
	SDL_RenderFillRect(renderer, &tableWallRight);
	SDL_RenderFillRect(renderer, &tableWallBottom);

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF); // Set colour black
	SDL_RenderDrawRect(renderer, &tableWallOutline);
}

/// Delete and remove all the balls marked for delete after being potted from the vector of balls.
/// Thus removing them from the table.
void GameEngine::deleteBalls()
{
	balls.erase(std::remove_if(balls.begin(), balls.end(),
		[this](Ball* b)
		{
			if (b->getDeleteFlag()) // If marked delete pointer
			{
				delete b;
				b = nullptr;
				return true;	// return true to remove from vector
			}
			else return false;
		}
	), balls.end());
}

/// Place all the balls into the default triangle position on screen ready for break of a new game.
/// Balls are centred around BLACK_POINT_X and BLACK_POINT_Y.
void GameEngine::placeNewBalls()
{
	balls.push_back(new Ball(BLACK_POINT_X - 40, BLACK_POINT_Y, SphereEntity::Colours::red));
	balls.push_back(new Ball(BLACK_POINT_X, BLACK_POINT_Y, SphereEntity::Colours::black)); // Black Ball

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

	balls.push_back(&whiteBall); // White Wall takes no position because it starts in the players hand ready to be dropped behind the line.
}

/// Saves the state of the table to file.
/// Saves all the balls positions (x and y values)
/// and all the player info.
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

	// Save game info is pulled out of the player object as a struct of information.
	Players::SaveVariables saveVar = players->getSaveVariables();

	playerFile << saveVar.isPlayer1Turn << endl;
	playerFile << saveVar.arePlayerColoursSetup << endl;
	playerFile << saveVar.player1.ShotsLeft << endl;
	playerFile << saveVar.player2.ShotsLeft << endl;
	playerFile << (int)saveVar.player1.Colour << endl;
	playerFile << (int)saveVar.player2.Colour << endl;
	for (auto& displayBall : saveVar.displayBalls)
	{
		playerFile << (int)displayBall.getColour() << endl; // All the display balls are saved to file
	}

	ballFile.close();
	playerFile.close();
}

/// Sets up the player and ball objects from file for the resume game functionality.
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
		// Ball info is pulled from file.
		ballFile >> x;
		ballFile >> y;
		ballFile >> colour;
		if (ballFile.eof()) break; // Balls are added to the ball vector until we reach the end of file.
		if ((SphereEntity::Colours)colour == SphereEntity::Colours::white)
		{
			whiteBall = WhiteBall(x, y, false, pockets);
			balls.push_back(&whiteBall);
		}
		else
		{
			balls.push_back(new Ball(x, y, (SphereEntity::Colours)colour));
		}
	}
	ballFile.close();

	ifstream playerFile("player.txt");

	// Player save game data is packaged into a struct and sent back to the players object.
	Players::SaveVariables saveVar;
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
	int colour3;
	while (true)
	{
		playerFile >> colour3;
		if (playerFile.eof()) break; // display balls are added to the vector until end off file.
		saveVar.displayBalls.push_back(Ball(0, 0, (SphereEntity::Colours)colour3));
	}

	players->setVariablesFromFile(saveVar); // Player save variables are set back in the players object

	playerFile.close();
}

/// Shows the user that save game dialogue that allows the player to choose
/// whether they want to save the game to file after quitting.
void GameEngine::saveGameDialogue()
{
	using namespace std;
	HWND hwnd = GetConsoleWindow();
	SetForegroundWindow(hwnd); // Brings the console window the front to allow save game functionality

	while (true) // Loops until user gives valid answer
	{
		string answer;
		cout << "Do You Wish To Save The Game? y or n" << endl;
		cin >> answer;
		cin.clear(); // clears and ignores values stored in cin for next loop
		cin.ignore(INT_MAX, '\n');
		switch (answer[0])
		{
		case 'y':
			saveStateOfTable(); // if the user inputs 'y' save the game to file.
			return;
		case 'n':
			return;
		default:
			cout << "Incorrect answer chosen. Place type 'y' or 'n' " << endl;
			break;
		}
	}
}

///	Loads a texture from a file.
/// Used to load in the ball and pocket textures.
SDL_Texture* GameEngine::loadTexture(const char* filePath)
{
	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = SDL_LoadBMP(filePath);

	if (loadedSurface == NULL) // If the surface didn't load properly print an error.
	{
		std::cout << "load image didn't work! " << SDL_GetError() << std::endl;
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL) // If the Texture didn't load properly print an error.
		{
			std::cout << "load texture didn't work! " << SDL_GetError() << std::endl;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}
	return newTexture;
}