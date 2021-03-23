#pragma once
#include <vector>

#include "City.h"
#include "DoublyLinkedList.h"

/* contains cities and provides an interface to access them */
class CityContainer
{
public:
	using str_type = const std::string&; // type less things
	void append_city(str_type name);
	void add_package_to_city(str_type package, str_type city_name);
	void add_truck_to_city(str_type truck_name, float truck_strength, str_type city_name);
	void execute_mission(str_type start, str_type midway, str_type target,
		int start_package_count, int midway_package_count, const std::vector<int>& midway_drop_indexes);

	std::string get_output() const;
	
private:
	City* find_city_by_name(str_type city_name);
	
	DoublyLinkedList<City> cities;
};