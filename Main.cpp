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
	std::ifstream file("balls.txt");

	while (file.good())
	{
		char answer;
		std::cout << "Do You Wish To Resume Your Last Game? y or n" << std::endl;
		std::cin >> answer;
		switch (answer)
		{
		case 'y':
			file.close();
			return true;
			break;
		case 'n':
			file.close();
			return false;
			break;
		default:
			std::cout << "Incorrect answer chosen. Place type 'y' or 'n' " << std::endl;
			break;
		}
	}
	file.close();
	return false;
}