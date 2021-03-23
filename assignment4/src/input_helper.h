#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>

namespace input_helper
{
	/* splits input by specified token */
	std::vector<std::string> split(std::string line, char token);

	/* erase all new line characters from the line */
	void erase_new_line_characters(std::string& line);

	/* returns a map which maps a character to its frequency */
	std::unordered_map<char, int> get_character_frequency_map(const std::string& text);
}
