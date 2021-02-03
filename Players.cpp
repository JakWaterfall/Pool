#include "Players.h"

Players::Players()
	: areColoursSetup(false), giveOtherPlayer2Shots(false), player1Turn(true), player1Shots(1), player2Shots(0), player1Colour(black), player2Colour(black)
{

}

void Players::update(WhiteBall& white, std::vector<Ball>& pottedBalls)
{
	
	if (white.info.endTurn)
	{
		resolvePottedBalls(pottedBalls);
		whiteHitOrMissOtherBall(white);

		if (giveOtherPlayer2Shots)
		{
 			resolve2Shots();
		}
		else
		{
			if (player1Turn)
			{
				if (--player1Shots < 1)
				{
					player2Shots = 1;
					player1Turn = false;
				}
			}
			else
			{
				if (--player2Shots < 1)
				{
					player1Shots = 1;
					player1Turn = true;
				}
			}
		}
		// reset white ball info
		white.info.hit = false;
		white.info.endTurn = false;
		white.info.hitOtherBall = false;
		pottedBalls.clear();


		using namespace std;
 		cout << "player turn: " << (player1Turn ? "1" : "2") << endl;
		cout << "shots left: " << (player1Turn ? player1Shots : player2Shots) << endl;
		string colour;
		if (player1Turn)
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
}

void Players::render(SDL_Renderer* renderer)
{

}

void Players::whiteHitOrMissOtherBall(WhiteBall& white)
{
	if (!white.info.hitOtherBall)
	{
		giveOtherPlayer2Shots = true;
	}
	else
	{
		switch (white.info.colourHitFirst)
		{
		case SphereEntity::Colours::red:
			if (player1Turn && player1Colour == SphereEntity::Colours::yellow)
			{
				giveOtherPlayer2Shots = true;
			}
			if (!player1Turn && player2Colour == SphereEntity::Colours::yellow)
			{
				giveOtherPlayer2Shots = true;
			}
			break;
		case SphereEntity::Colours::yellow:
			if (player1Turn && player1Colour == SphereEntity::Colours::red)
			{
				giveOtherPlayer2Shots = true;
			}
			if (!player1Turn && player2Colour == SphereEntity::Colours::red)
			{
				giveOtherPlayer2Shots = true;
			}
			break;
		case SphereEntity::Colours::black:
				giveOtherPlayer2Shots = true;
				break;
		default:
			std::cout << "something went wrong";
			break;
		}
	}
}

void Players::resolvePottedBalls(std::vector<Ball>& pottedBalls)
{
	if (!pottedBalls.empty())
	{
		if (!areColoursSetup)
			setupColours(pottedBalls);


		SphereEntity::Colours firstColour = pottedBalls[0].getColour();

		if (player1Turn)
		{
			if (player1Colour == firstColour)
			{
				player1Shots++;
			}
			else if (player1Colour != black)
			{
				giveOtherPlayer2Shots = true;
			}
		}
		else
		{
			if (player2Colour == firstColour)
			{
				player2Shots++;
			}
			else if(player2Colour != black)
			{
				giveOtherPlayer2Shots = true;
			}
		}

		for (auto& b : pottedBalls)
		{
			if (b.getColour() == white)
			{
				giveOtherPlayer2Shots = true;
			}

			if (b.getColour() == black)
			{
				if (player1Turn)
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
	SphereEntity::Colours firstColour = pottedBalls[0].getColour();

	if (firstColour != white && firstColour != black)
	{
		SphereEntity::Colours secondColour = firstColour == red ? yellow : red;
		if (player1Turn)
		{
			player1Colour = firstColour;
			player2Colour = secondColour;
		}
		else
		{
			player2Colour = firstColour;
			player1Colour = secondColour;
		}
		areColoursSetup = true;
	}
}

void Players::resolve2Shots()
{
	if (player1Turn)
	{
		player2Shots = 2;
		player1Turn = false;
	}
	else
	{
		player1Shots = 2;
		player1Turn = true;
	}
	giveOtherPlayer2Shots = false;
}

