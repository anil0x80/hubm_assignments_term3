#include <algorithm>
#include <iostream>


#include "CityContainer.h"
#include "InputHelper.h"

int main(int argc, char* argv[])
{
	using std::cout;
	using std::endl;
	using std::string;

	if (argc < 6)
	{
		cout << "Too few arguments!" << endl << "Usage: " << argv[0] << " dests.txt packages.txt trucks.txt missions.txt result.txt" << endl;
		return 0;
	}
	
	CityContainer cities;
	
	std::ifstream input_dests(argv[1]); // contains cities 
	std::ifstream input_packages(argv[2]); // contains packages and their places 
	std::ifstream input_trucks(argv[3]); // trucks and their places 
	std::ifstream input_missions(argv[4]); // commands

	/* set up all cities and related data */
	for (string city_name; std::getline(input_dests, city_name); )
	{
		InputHelper::erase_new_lines(city_name);
		cities.append_city(city_name);
	}

	for (string line; std::getline(input_packages, line); )
	{
		InputHelper::erase_new_lines(line);
		auto tokens = InputHelper::split(line, ' ');
		cities.add_package_to_city(tokens[0], tokens[1]);
	}

	for (string line; std::getline(input_trucks, line); )
	{
		InputHelper::erase_new_lines(line);
		auto tokens = InputHelper::split(line, ' ');
		cities.add_truck_to_city(tokens[0], std::stof(tokens[2]) ,tokens[1]);
	}

	for (string line; std::getline(input_missions, line); )
	{
		InputHelper::erase_new_lines(line);
		auto tokens = InputHelper::split(line, '-');

		std::vector<int> midway_drop_indexes;
		if (tokens.size() > 5)
		{
			/* transform string ("3", "4", "5") to int (3, 4, 5) */
			auto midway_drop_indexes_str = InputHelper::split(tokens[5], ',');
			std::transform(std::begin(midway_drop_indexes_str),
				std::end(midway_drop_indexes_str),
				std::back_inserter(midway_drop_indexes),
				[](const string& s) { return std::stoi(s); }
			);
		}
		
		cities.execute_mission(tokens[0], tokens[1], tokens[2], 
			std::stoi(tokens[3]),
			std::stoi(tokens[4]), midway_drop_indexes
			);
	}

	std::ofstream output_file(argv[5]);
	output_file << cities.get_output();
	
	return 0;
}
