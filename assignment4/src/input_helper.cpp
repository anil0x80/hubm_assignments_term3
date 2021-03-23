#include "input_helper.h"
#include <sstream>
#include <algorithm>
#include <iostream>

std::vector<std::string> input_helper::split(std::string line, char token)
{
	/* split string by token, because std was lazy to implement it, we need to implement it ourselves..*/
	std::vector<std::string> parts;
	std::istringstream iss(line);
	std::string part;
	while (std::getline(iss, part, token))
		parts.push_back(part);

	return parts;
}

void input_helper::erase_new_line_characters(std::string& line)
{
	/* make sure line endings does not cause problems on linux */
	/* do not forget to include algorithm library or code won't compile on g++ */
	line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
	line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
}

std::unordered_map<char, int> input_helper::get_character_frequency_map(const std::string& text)
{	
	std::unordered_map <char, int> frequency_data;

	for (char const& c : text) {
		frequency_data[c]++;
	}

	return frequency_data;
}
