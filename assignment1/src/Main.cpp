#include "GameBoard.h"
#include <iostream>

int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		std::cout << "Usage: " << argv[0] << " <deck_file> <commands_file> <output_file>" << std::endl;
		return 0;
	}
	
	GameBoard game_board(argv[1]);
	game_board.run_commands(argv[2], argv[3]);
	return 0;
}
