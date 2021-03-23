#include "LibrarySystem.h"
#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

/* splits input by tabs */
std::vector<std::string> tokenize(std::string line)
{
	std::vector<std::string> tokens;
	std::istringstream iss(line);
	std::string token;
	while (std::getline(iss, token, '\t'))
		tokens.push_back(token);

	return tokens;
}

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout << "Usage: " << argv[0] << " <commands_file> <output_file>" << std::endl;
		return 0;
	}
	
	
	std::ifstream input(argv[1]);
	LibrarySystem library(argv[2]);
	
	for (std::string line; std::getline(input, line); )
	{
		/* make sure stupid line endings does not cause problems */
		line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
		line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
		
		auto tokens = tokenize(line);
		auto& command = tokens[0];

		if(command == "addMovie")
		{
			library.addMovie(std::stoi(tokens[1]), tokens[2], std::stoi(tokens[3]));
		}
		else if (command == "deleteMovie")
		{
			library.deleteMovie(std::stoi(tokens[1]));
		}
		else if (command == "addUser")
		{
			library.addUser(std::stoi(tokens[1]), tokens[2]);
		}
		else if (command == "deleteUser")
		{
			library.deleteUser(std::stoi(tokens[1]));
		}
		else if (command == "checkoutMovie")
		{
			library.checkoutMovie(std::stoi(tokens[1]), std::stoi(tokens[2]));
		}
		else if (command == "showUser")
		{
			library.showUser(std::stoi(tokens[1]));
		}
		else if (command == "showMovie")
		{
			library.showMovie(std::stoi(tokens[1]));
		}
		else if (command == "showAllMovie")
		{
			library.showAllMovies();
		}
		else if (command == "returnMovie")
		{
			library.returnMovie(std::stoi(tokens[1]));
		}
		else
		{
			/* unknown command !*/
			std::cout << "Parser, unknown command: " << command << std::endl;
		}

		/* after each command save the changes to disk */
		library.flush_output();
	}
	
	return 0;
}
