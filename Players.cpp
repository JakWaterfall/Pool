#include "Players.h"

Players::Players()
	: arePlayerColoursSetup(false), foulBall(false), isPlayer1Turn(true), gameOver(false), player1Won(false), player1({1, black}), player2({ 0, black })
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

void Players::update(WhiteBall& white, std::vector<Ball>& pottedBalls, std::vector<Ball*>& balls)
{
	if (white.info.endTurn)
	{
		whiteHitOrMissOtherBall(white, balls);
		resolvePottedBalls(pottedBalls, balls, white);
		resolvePlayerTurn();
		
		for (auto& b : pottedBalls)
		{
			if(b.getColour() != SphereEntity::Colours::white)
				displayBalls.push_back(b);
		}
		// reset white ball info
		pottedBalls.clear();
		white.resetInfo();

		debugConsoleLogInfo(); // debug
	}
}

void Players::render(SDL_Renderer* renderer)
{
	renderDisplayBalls(renderer);
	std::string newScring;
	if (!gameOver)
	{
		newScring = "Player: ";
		newScring += (isPlayer1Turn) ? "1" : "2";
		renderText(renderer, newScring.c_str(), 10, 0);

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

		newScring = "Shots Left: ";
		newScring += std::to_string(getCurrentPlayer().ShotsLeft);
		renderText(renderer, newScring.c_str(), 10, 60);
	}
	else
	{
		newScring = "WINNER!!! Player: ";
		newScring += (player1Won) ? "1" : "2";
		renderText(renderer, newScring.c_str(), 500, 25);
	}
}

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

void Players::renderDisplayBalls(SDL_Renderer* renderer)
{
	int i = 0;
	for (auto& ball : displayBalls)
	{
		SDL_Rect frame = { 250 + i, 50, (int)ball.getRadius() * 2, (int)ball.getRadius() * 2 };
		SDL_RenderCopy(renderer, ball.getTexture(), NULL, &frame);
		i = i + (int)ball.getRadius() * 2;
	}
}

void Players::whiteHitOrMissOtherBall(WhiteBall& white, std::vector<Ball*>& balls)
{
	if (!white.info.hitOtherBall)
	{
		foulBall = true;
	}
	else
	{
		switch (white.info.colourHitFirst)
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
		case SphereEntity::Colours::black: // impliment if u hit black and it your last ball you dont foul.
			for (auto& ball : balls) // checks if player has a ball left on the table
			{
				if (ball->getColour() == getCurrentPlayer().Colour)
				{
					foulBall = true;
					break;
				}
			}
				break;
		default:
			std::cout << "something went wrong with hiting other ball flag" << std::endl;
			break;
		}
	}
}

void Players::resolvePottedBalls(std::vector<Ball>& pottedBalls, std::vector<Ball*>& balls, WhiteBall& whiteBall)
{
	if (!pottedBalls.empty())
	{
		SphereEntity::Colours firstBallColour = pottedBalls[0].getColour();
		
		if (!arePlayerColoursSetup)
			setupColours(pottedBalls, firstBallColour);

		if (getCurrentPlayer().Colour == firstBallColour)
		{
			getCurrentPlayer().ShotsLeft++;
		}
		else
		{
			foulBall = true;
		}

		for (size_t i = 0; i < pottedBalls.size(); i++)
		{
			if (pottedBalls[i].getColour() == white)
			{
				foulBall = true;
			}

			if (pottedBalls[i].getColour() == black)
			{
				resolveWinner(pottedBalls, balls, whiteBall, i);	
			}
		}
	}
}

void Players::resolveWinner(std::vector<Ball>& pottedBalls, std::vector<Ball*>& balls, WhiteBall& whiteBall, int blackIndex)
{
	using namespace std; // DEGBUG REMOVE !!!

	bool win = true;
	gameOver = true;
	SphereEntity::Colours playerColour = getCurrentPlayer().Colour;

	if (playerColour == black) // this checks if they potted the black ball before they have a colour assosiated to them
		win = false;

	if (whiteBall.info.colourHitFirst == getOtherPlayer().Colour) // check player didnt hit opponants ball first before potting black
		win = false;

	for (auto& ball : balls) // checks if player has a ball left on the table
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
		if (pottedBalls[j].getColour() == white)
		{
			win = false;
			break;
		}

		if (pottedBalls[j].getColour() == playerColour)
			lastColourIndex = j;
	}
	if (lastColourIndex > blackIndex) // this checks if the player potted the black ball first before thier last colour ball.
		win = false;

	if (win)
		isPlayer1Turn ? player1Won = true : player1Won = false;
	else
		!isPlayer1Turn ? player1Won = true : player1Won = false;

	cout << "did player win: " << win << endl;
	cout << "last colour index: " << lastColourIndex << endl;
	cout << "black Index: " << blackIndex << endl;
}

void Players::setupColours(std::vector<Ball>& pottedBalls, SphereEntity::Colours & firstBallColour)
{
	if (firstBallColour != white && firstBallColour != black)
	{
		SphereEntity::Colours secondBallColour = firstBallColour == red ? yellow : red;

		getCurrentPlayer().Colour = firstBallColour;
		getOtherPlayer().Colour = secondBallColour;

		arePlayerColoursSetup = true;
	}
}

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

void Players::debugConsoleLogInfo()
{
	// DEGUB
	using namespace std;
	cout << "player turn: " << (isPlayer1Turn ? "1" : "2") << endl;
	cout << "shots left: " << getCurrentPlayer().ShotsLeft << endl;

	cout << "colour: " << getColourString() << endl << endl;
}

std::string Players::getColourString()
{
	switch (getCurrentPlayer().Colour)
	{
	case SphereEntity::Colours::white:
		return "white";
		break;
	case SphereEntity::Colours::black:
		return "Free Colour";
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
	return { arePlayerColoursSetup, isPlayer1Turn, player1, player2 };
}

void Players::setVariablesFromFile(saveVariables saveVar)
{
	arePlayerColoursSetup = saveVar.arePlayerColoursSetup;
	isPlayer1Turn = saveVar.isPlayer1Turn;
	player1 = saveVar.player1;
	player2 = saveVar.player2;
}

bool Players::getGameOver()
{
	return gameOver;
}
