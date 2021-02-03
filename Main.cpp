#include "GameEngine.h"

bool checkResumingGame();

int main(int argc, char* args[])
{
	bool resume = checkResumingGame();

	// Starts Game Engine
	GameEngine game(resume);
	return 0;
}

bool checkResumingGame()
{
	std::ifstream ballFile("balls.txt");
	std::ifstream playersFile("player.txt");

	while (ballFile.good() && playersFile.good())
	{
		char answer;
		std::cout << "Do You Wish To Resume Your Last Game? y or n" << std::endl;
		std::cin >> answer;
		switch (answer)
		{
		case 'y':
			ballFile.close();
			playersFile.close();
			return true;
			break;
		case 'n':
			ballFile.close();
			playersFile.close();
			return false;
			break;
		default:
			std::cout << "Incorrect answer chosen. Place type 'y' or 'n' " << std::endl;
			break;
		}
	}
	ballFile.close();
	playersFile.close();
	return false;
}