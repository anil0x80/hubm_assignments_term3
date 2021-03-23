#pragma once
#include <string>
#include <vector>
#include <fstream>


namespace InputHelper
{
	/* splits input by specified token */
	std::vector<std::string> split(std::string line, char token);

	/* erase all new line characters from the line */
	void erase_new_lines(std::string& line);
}
