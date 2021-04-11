#include "GameEngine.h"

bool checkResumingGame();
int getPocketSize();

int main(int argc, char* args[])
{
	bool resume = checkResumingGame();
	int pocketSize = getPocketSize();

	// Starts Game Engine
	GameEngine game(resume, pocketSize);

	return 0;
}

bool checkResumingGame()
{
	std::ifstream ballFile("balls.txt");
	std::ifstream playersFile("player.txt");

	while (ballFile.good() && playersFile.good())
	{
		std::string answer;
		std::cout << "Do You Wish To Resume Your Last Game? y or n" << std::endl;
		std::cin >> answer;
		switch (answer[0])
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

int getPocketSize()
{
	while (true)
	{
		std::string answer;
		std::cout << "Choose Pocket Size:" << std::endl;
		std::cout << "1 = Small | 2 = Medium | 3 = Large " << std::endl;
		std::cin >> answer;
		switch (answer[0])
		{
		case '1':
			return 20;
			break;
		case '2':
			return 25;
			break;
		case '3':
			return 30;
			break;
		default:
			std::cout << "\nIncorrect answer chosen.\n" << std::endl;
			break;
		}
	}
}
