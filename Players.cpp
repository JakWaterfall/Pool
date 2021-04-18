#include "Players.h"

/// <summary>
/// Inits variables and Fonts for text on screen.
/// </summary>
Players::Players()
	: arePlayerColoursSetup(false), foulBall(false), isPlayer1Turn(true), gameOver(false), player1Won(false), player1({ 1, black }), player2({ 0, black })
{
	font = TTF_OpenFont("Fonts/arial.ttf", 28);
	if (font == NULL)
	{
		std::cout << "font failed to load " << TTF_GetError() << std::endl;
	}
}

Players::~Players()
{
	// free SDL pointers
	if (textSurface)
	{
		SDL_FreeSurface(textSurface);
		textSurface = NULL;
	}
	if (textTexture)
	{
		SDL_DestroyTexture(textTexture);
		textTexture = NULL;
	}
	if (font)
	{
		TTF_CloseFont(font);
		font = NULL;
	}
}

/// <summary>
/// At the end of every turn, all the variables that make up the turn action are resolved including
/// Whether the white ball hit another ball, which balls had been potted and in which order and
/// resolve whether the current player had a foul ball or not.
/// </summary>
/// <param name="white">White Ball.</param>
/// <param name="pottedBalls">Balls that have been potted this turn.</param>
/// <param name="balls">All the alls on the table.</param>
void Players::update(WhiteBall& white, std::vector<Ball>& pottedBalls, std::vector<Ball*>& balls)
{
	if (white.info.endTurn)
	{
		whiteHitOrMissOtherBall(white, balls);
		resolvePottedBalls(pottedBalls, balls, white);
		resolvePlayerTurn();

		for (auto& b : pottedBalls)
		{
			if (b.getColour() != SphereEntity::Colours::white) // Display all the potted balls except the white balls.
				displayBalls.push_back(b);
		}
		// reset white ball info & clear potted balls ready for next turn.
		pottedBalls.clear();
		white.resetInfo();
	}
}

/// <summary>
///	Renders relevant player info to the screen such current player turn, their colour and how many shots left.
/// Also renders end game text of which player won.
/// </summary>
/// <param name="renderer">SDL renderer object that draws to screen.</param>
void Players::render(SDL_Renderer* renderer)
{
	renderDisplayBalls(renderer);
	std::string newScring;
	if (!gameOver)
	{
		// Renders who's turn it is
		newScring = "Player: ";
		newScring += (isPlayer1Turn) ? "1" : "2";
		renderText(renderer, newScring.c_str(), 10, 0);

		// Renders current players colour
		newScring = "Colour: " + getColourString();
		SDL_Color textColour;
		if (getColourString() == "Red")
		{
			textColour = { 255, 0, 0 };
		}
		else if (getColourString() == "Yellow")
		{
			textColour = { 230, 230, 0 };
		}
		else
		{
			textColour = { 0, 0, 0 };
		}
		renderText(renderer, newScring.c_str(), 10, 30, textColour);

		// Renders number of shots left
		newScring = "Shots Left: ";
		newScring += std::to_string(getCurrentPlayer().ShotsLeft);
		renderText(renderer, newScring.c_str(), 10, 60);
	}
	else
	{
		// Renders winning player text
		newScring = "WINNER!!! Player: ";
		newScring += (player1Won) ? "1" : "2";
		renderText(renderer, newScring.c_str(), 500, 25);
	}
}

/// <summary>
///	Renders text to screen given the string, position (x,y) and text colour.
/// </summary>
/// <param name="renderer">SDL renderer object that draws to screen.</param>
/// <param name="text">String to print.</param>
/// <param name="x">X value on screen.</param>
/// <param name="y">Y value on screen.</param>
/// <param name="textColour">Text colour.</param>
void Players::renderText(SDL_Renderer* renderer, const char* text, int x, int y, SDL_Color textColour)
{
	textSurface = TTF_RenderText_Solid(font, text, textColour);
	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	int width = textSurface->w;
	int height = textSurface->h;
	SDL_Rect renderQuad = { x, y, width, height };
	SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);

	// free space
	SDL_FreeSurface(textSurface);
	textSurface = NULL;
	SDL_DestroyTexture(textTexture);
	textTexture = NULL;
}

/// <summary>
/// Renders the display balls (balls that have been potted) to screen.
/// </summary>
/// <param name="renderer">SDL renderer object that draws to screen.</param>
void Players::renderDisplayBalls(SDL_Renderer* renderer)
{
	int i = 0;
	for (auto& ball : displayBalls) // Loops though all the balls, offsetting them by the diameter of the ball and rendering.
	{
		int ballDiameter = (int)ball.getRadius() * 2;
		SDL_Rect ballFrame = { 250 + i, 50, ballDiameter, ballDiameter };
		SDL_RenderCopy(renderer, ball.getTexture(), NULL, &ballFrame);
		i = i + ballDiameter; // i is incremented up by the diameter of the balls.
	}
}

/// <summary>
/// Checks for foul balls.
/// Checks by testing if the white ball hit another ball or a ball which was not the players colour first.
/// </summary>
/// <param name="white">White Ball.</param>
/// <param name="balls">All the alls on the table.</param>
void Players::whiteHitOrMissOtherBall(WhiteBall& white, std::vector<Ball*>& balls)
{
	if (!white.info.hitOtherBall)
	{
		foulBall = true; // Foul if player missed every ball.
	}
	else
	{
		switch (white.info.colourHitFirst) // Foul ball if player colour is set and they hit the wrong colour ball
		{
		case SphereEntity::Colours::red:
			if (getCurrentPlayer().Colour == yellow)
			{
				foulBall = true;
			}
			break;
		case SphereEntity::Colours::yellow:
			if (getCurrentPlayer().Colour == red)
			{
				foulBall = true;
			}
			break;
		case SphereEntity::Colours::black:
			for (auto& ball : balls) // Checks if player has a ball left on the table
			{
				if (ball->getColour() == getCurrentPlayer().Colour)
				{
					foulBall = true; // Foul ball if a player hits the black ball first when they have other balls on the table left.
					break;
				}
			}
			break;
		default:
			break;
		}
	}
}

/// <summary>
///	Resolves the turn action in relation to the balls that had been potted.
/// Sets up player colours if they have not been set.
/// </summary>
/// <param name="pottedBalls">Balls that have been potted this turn.</param>
/// <param name="balls">All the alls on the table.</param>
/// <param name="whiteBall">White Ball.</param>
void Players::resolvePottedBalls(std::vector<Ball>& pottedBalls, std::vector<Ball*>& balls, WhiteBall& whiteBall)
{
	if (!pottedBalls.empty()) // If any balls have been potted.
	{
		SphereEntity::Colours firstBallColour = pottedBalls[0].getColour();

		if (!arePlayerColoursSetup)
			setupColours(pottedBalls, firstBallColour); // Set up player colours.

		if (getCurrentPlayer().Colour == firstBallColour)
		{
			getCurrentPlayer().ShotsLeft++; // If player pockets one of their balls first give them extra shot.
		}
		else
		{
			foulBall = true; // If player pockets any other ball first they foul.
		}

		for (size_t i = 0; i < pottedBalls.size(); i++) // Check every potted ball.
		{
			if (pottedBalls[i].getColour() == white)
			{
				foulBall = true; // If player pockets white ball they foul.
			}

			if (pottedBalls[i].getColour() == black)
			{
				resolveWinner(pottedBalls, balls, whiteBall, i); // If player pockets black ball the game is over.
			}
		}
	}
}

/// <summary>
///	Resolve who won the game by analysing how many balls the player has left on the table, which ball they hit first and in what order did they pot each ball.
/// </summary>
/// <param name="pottedBalls">Balls that have been potted this turn.</param>
/// <param name="balls">All the alls on the table.</param>
/// <param name="whiteBall">White Ball.</param>
/// <param name="blackIndex">Black ball position in the potted balls vector.</param>
void Players::resolveWinner(std::vector<Ball>& pottedBalls, std::vector<Ball*>& balls, WhiteBall& whiteBall, int blackIndex)
{
	bool win = true;
	gameOver = true;
	SphereEntity::Colours playerColour = getCurrentPlayer().Colour;

	if (playerColour == black) // Lost if player potted the black ball before player has a colour associated to them.
		win = false;

	if (whiteBall.info.colourHitFirst == getOtherPlayer().Colour) // Lost if player hit opponents ball first before potting black.
		win = false;

	for (auto& ball : balls) // Lost if player has a ball left on the table.
	{
		if (ball->getColour() == playerColour)
		{
			win = false;
			break;
		}
	}

	int lastColourIndex = -1;
	for (size_t j = 0; j < pottedBalls.size(); j++)
	{
		if (pottedBalls[j].getColour() == white) // Lost if player potted the white ball on the same turn as the black ball.
		{
			win = false;
			break;
		}

		if (pottedBalls[j].getColour() == playerColour)
			lastColourIndex = j;
	}
	if (lastColourIndex > blackIndex) // Lost if player potted the black ball first before their last colour ball.
		win = false;

	if (win) // Test which player won by who's turn it currently is.
		isPlayer1Turn ? player1Won = true : player1Won = false;
	else
		isPlayer1Turn ? player1Won = false : player1Won = true;
}

/// <summary>
///	Sets up the players colours based on which colour was potted first.
/// </summary>
/// <param name="pottedBalls">Balls that have been potted this turn.</param>
/// <param name="firstBallColour">Colour of the first ball potted.</param>
void Players::setupColours(std::vector<Ball>& pottedBalls, SphereEntity::Colours& firstBallColour)
{
	if (firstBallColour != white && firstBallColour != black) // Checks first colour wasn't a foul ball.
	{
		SphereEntity::Colours secondBallColour = firstBallColour == red ? yellow : red;

		getCurrentPlayer().Colour = firstBallColour;
		getOtherPlayer().Colour = secondBallColour;

		arePlayerColoursSetup = true;
	}
}

/// <summary>
/// Resolves player turn. If current player got a foul ball they give the opposing player 2 shots and change turns
/// otherwise they use up a shot and if they have less than 1 shot left they change turns.
/// </summary>
void Players::resolvePlayerTurn()
{
	if (foulBall)
	{
		getOtherPlayer().ShotsLeft = 2;
		foulBall = false;
		isPlayer1Turn = !isPlayer1Turn;
	}
	else
	{
		if (--getCurrentPlayer().ShotsLeft < 1)
		{
			getOtherPlayer().ShotsLeft = 1;
			isPlayer1Turn = !isPlayer1Turn;
		}
	}
}

Players::Player& Players::getCurrentPlayer()
{
	return isPlayer1Turn ? player1 : player2;
}

Players::Player& Players::getOtherPlayer()
{
	return isPlayer1Turn ? player2 : player1;
}

/// <summary>
/// Returns string of the current players colour.
/// </summary>
/// <returns>String of the current players colour</returns>
std::string Players::getColourString()
{
	switch (getCurrentPlayer().Colour)
	{
	case SphereEntity::Colours::white:
		return "white";
		break;
	case SphereEntity::Colours::black:
		return "Free Colour"; // Players are given the colour of black as default before colours are set up.
		break;
	case SphereEntity::Colours::red:
		return "Red";
		break;
	case SphereEntity::Colours::yellow:
		return "Yellow";
		break;
	default:
		return "ERROR";
		break;
	}
}

Players::saveVariables Players::getSaveVariables()
{
	return { arePlayerColoursSetup, isPlayer1Turn, player1, player2, displayBalls };
}

void Players::setVariablesFromFile(saveVariables saveVar)
{
	arePlayerColoursSetup = saveVar.arePlayerColoursSetup;
	isPlayer1Turn = saveVar.isPlayer1Turn;
	player1 = saveVar.player1;
	player2 = saveVar.player2;
	displayBalls = saveVar.displayBalls;
}

bool Players::getGameOver()
{
	return gameOver;
}