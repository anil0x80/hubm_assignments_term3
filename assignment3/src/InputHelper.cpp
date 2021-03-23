#include "InputHelper.h"
#include <sstream>
#include <algorithm>

std::vector<std::string> InputHelper::split(std::string line, char token)
{
	std::vector<std::string> parts;
	std::istringstream iss(line);
	std::string part;
	while (std::getline(iss, part, token))
		parts.push_back(part);

	return parts;
}

void InputHelper::erase_new_lines(std::string& line)
{
	/* make sure line endings does not cause problems on linux */
	/* do not forget to include algorithm library or code won't compile on g++ */
	line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
	line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
}
