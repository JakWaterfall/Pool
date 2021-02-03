#include "Players.h"

Players::Players()
	: arePlayerColoursSetup(false), foulBall(false), isPlayer1Turn(true), player1ShotsLeft(1), player2ShotsLeft(0), player1Colour(black), player2Colour(black)
{

}

void Players::update(WhiteBall& white, std::vector<Ball>& pottedBalls)
{
	
	if (white.info.endTurn)
	{
		resolvePottedBalls(pottedBalls);
		whiteHitOrMissOtherBall(white);
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
	// render turns and score to screen
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
			if (isPlayer1Turn && player1Colour == yellow || !isPlayer1Turn && player2Colour == yellow)
			{
				foulBall = true;
			}
			break;
		case SphereEntity::Colours::yellow:
			if (isPlayer1Turn && player1Colour == red || !isPlayer1Turn && player2Colour == red)
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
		if (!arePlayerColoursSetup)
			setupColours(pottedBalls);

		SphereEntity::Colours firstBallColour = pottedBalls[0].getColour();

		if (isPlayer1Turn)
		{
			if (player1Colour == firstBallColour)
			{
				player1ShotsLeft++;
			}
			else //if (arePlayerColoursSetup) // (player1Colour != black) dont think this is needed
			{
				foulBall = true;
			}
		}
		else
		{
			if (player2Colour == firstBallColour)
			{
				player2ShotsLeft++;
			}
			else //if (arePlayerColoursSetup) //(player2Colour != black)
			{
				foulBall = true;
			}
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
					std::cout << "player 1 loses" << std::endl;
				}
				else
				{
					std::cout << "player 2 loses" << std::endl;
				}
			}
		}
	}
}

void Players::setupColours(std::vector<Ball>& pottedBalls)
{
	SphereEntity::Colours firstBallColour = pottedBalls[0].getColour();

	if (firstBallColour != white && firstBallColour != black)
	{
		SphereEntity::Colours secondBallColour = firstBallColour == red ? yellow : red;
		if (isPlayer1Turn)
		{
			player1Colour = firstBallColour;
			player2Colour = secondBallColour;
		}
		else
		{
			player2Colour = firstBallColour;
			player1Colour = secondBallColour;
		}
		arePlayerColoursSetup = true;
	}
}

void Players::resolveFoulBall()
{
	if (isPlayer1Turn)
	{
		player2ShotsLeft = 2;
		isPlayer1Turn = false;
	}
	else
	{
		player1ShotsLeft = 2;
		isPlayer1Turn = true;
	}
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
		if (isPlayer1Turn)
		{
			if (--player1ShotsLeft < 1)
			{
				player2ShotsLeft = 1;
				isPlayer1Turn = false;
			}
		}
		else
		{
			if (--player2ShotsLeft < 1)
			{
				player1ShotsLeft = 1;
				isPlayer1Turn = true;
			}
		}
	}
}

void Players::debugConsoleLogInfo()
{
	// DEGUB
	using namespace std;
	cout << "player turn: " << (isPlayer1Turn ? "1" : "2") << endl;
	cout << "shots left: " << (isPlayer1Turn ? player1ShotsLeft : player2ShotsLeft) << endl;
	string colour;
	if (isPlayer1Turn)
	{
		switch (player1Colour)
		{
		case SphereEntity::Colours::white:
			colour = "white";
			break;
		case SphereEntity::Colours::black:
			colour = "black";
			break;
		case SphereEntity::Colours::red:
			colour = "red";
			break;
		case SphereEntity::Colours::yellow:
			colour = "yelow";
			break;
		default:
			break;
		}
	}
	else
	{
		switch (player2Colour)
		{
		case SphereEntity::Colours::white:
			colour = "white";
			break;
		case SphereEntity::Colours::black:
			colour = "black";
			break;
		case SphereEntity::Colours::red:
			colour = "red";
			break;
		case SphereEntity::Colours::yellow:
			colour = "yellow";
			break;
		default:
			break;
		}
	}
	cout << "colour: " << colour << endl << endl;
}

Players::saveVariables Players::getSaveVariables()
{
	return { arePlayerColoursSetup, isPlayer1Turn, player1ShotsLeft, player2ShotsLeft, player1Colour, player2Colour };
}

void Players::setVariablesFromFile(saveVariables saveVar)
{
	arePlayerColoursSetup = saveVar.arePlayerColoursSetup;
	isPlayer1Turn = saveVar.isPlayer1Turn;
	player1ShotsLeft = saveVar.player1ShotsLeft;
	player2ShotsLeft = saveVar.player2ShotsLeft;
	player1Colour = saveVar.player1Colour;
	player2Colour = saveVar.player2Colour;
}