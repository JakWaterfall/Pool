#include "Players.h"

Players::Players()
	: arePlayerColoursSetup(false), foulBall(false), isPlayer1Turn(true), player1({1, black}), player2({ 0, black })
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
	SDL_FreeSurface(textSurface);
	textSurface = NULL;
	SDL_DestroyTexture(TextTexture);
	TextTexture = NULL;
	TTF_CloseFont(font);
	font = NULL;
}

void Players::update(WhiteBall& white, std::vector<Ball>& pottedBalls)
{
	
	if (white.info.endTurn)
	{
		whiteHitOrMissOtherBall(white);
		resolvePottedBalls(pottedBalls);
		resolvePlayerTurn();
		
		// reset white ball info
		white.info.hit = false;
		white.info.endTurn = false;
		white.info.hitOtherBall = false;
		pottedBalls.clear();

		debugConsoleLogInfo(); // debug
	}
}

void Players::render(SDL_Renderer* renderer)
{
	// sort this mess out
	std::string newScring = "Player: "; 
	newScring += (isPlayer1Turn) ? "1" : "2";
	renderText(renderer, newScring.c_str(), 0, 0);

	newScring = "Colour: " + colour;
	renderText(renderer, newScring.c_str(), 0, 30);

	newScring = "Shots Left: "; 
	newScring += std::to_string(getCurrentPlayer().ShotsLeft);
	renderText(renderer, newScring.c_str(), 0, 60);
}

void Players::renderText(SDL_Renderer* renderer, const char* text, int x, int y)
{
	textSurface = TTF_RenderText_Solid(font, text, textColor);
	TextTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	int width = textSurface->w;
	int height = textSurface->h;
	SDL_Rect renderQuad = { x, y, width, height };
	SDL_RenderCopy(renderer, TextTexture, NULL, &renderQuad);
}

void Players::whiteHitOrMissOtherBall(WhiteBall& white)
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
			if (getCurrentPlayer().Colour == yellow) //(isPlayer1Turn && player1Colour == yellow || !isPlayer1Turn && player2Colour == yellow)
			{
				foulBall = true;
			}
			break;
		case SphereEntity::Colours::yellow:
			if (getCurrentPlayer().Colour == red) // (isPlayer1Turn && player1Colour == red || !isPlayer1Turn && player2Colour == red)
			{
				foulBall = true;
			}
			break;
		case SphereEntity::Colours::black: // impliment if u hit black and it your last ball you dont foul.
				foulBall = true;
				break;
		default:
			std::cout << "something went wrong with hiting other ball flag" << std::endl;
			break;
		}
	}
}

void Players::resolvePottedBalls(std::vector<Ball>& pottedBalls)
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

		for (auto& b : pottedBalls)
		{
			if (b.getColour() == white)
			{
				foulBall = true;
			}

			if (b.getColour() == black)
			{
				if (isPlayer1Turn)
				{
					foulBall = true;
					std::cout << "player 1 loses" << std::endl;
				}
				else
				{
					foulBall = true;
					std::cout << "player 2 loses" << std::endl;
				}
			}
		}
	}
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

void Players::resolveFoulBall()
{
	getOtherPlayer().ShotsLeft = 2;
	isPlayer1Turn = !isPlayer1Turn;
	foulBall = false;
}

void Players::resolvePlayerTurn()
{
	if (foulBall)
	{
		resolveFoulBall();
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
	colour = "";

	switch (getCurrentPlayer().Colour)
	{
	case SphereEntity::Colours::white:
		colour = "white";
		break;
	case SphereEntity::Colours::black:
		colour = "Free Colour";
		break;
	case SphereEntity::Colours::red:
			colour = "Red";
		break;
	case SphereEntity::Colours::yellow:
		colour = "Yellow";
		break;
	default:
		break;
	}

	cout << "colour: " << colour << endl << endl;
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