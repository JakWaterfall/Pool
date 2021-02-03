#include "Players.h"

Players::Players()
	: arePlayerColoursSetup(false), foulBall(false), isPlayer1Turn(true), player1({1, black}), player2({ 0, black })//player1ShotsLeft(1), player2ShotsLeft(0), player1Colour(black), player2Colour(black)
{

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
	string colour;

	switch (getCurrentPlayer().Colour)
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