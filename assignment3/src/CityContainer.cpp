#include "CityContainer.h"


#include <algorithm>
#include <iostream>
#include <sstream>

void CityContainer::append_city(str_type name)
{
	const City city(name);
	cities.append(city);
}

void CityContainer::add_package_to_city(str_type package, str_type city_name)
{
	/* find the city with name city_name, and use the interface to push the package */
	find_city_by_name(city_name)->push_cargo_package(package);
}

void CityContainer::add_truck_to_city(str_type truck_name, float truck_strength, str_type city_name)
{
	/* find the city with name city_name, and use the interface to queue the truck */
	find_city_by_name(city_name)->enqueue_cargo_truck({ truck_name, truck_strength });
}

void CityContainer::execute_mission(str_type start, str_type midway, str_type target, int start_package_count,
	int midway_package_count, const std::vector<int>& midway_drop_indexes)
{
	City* city_start = find_city_by_name(start);
	City* city_mid = find_city_by_name(midway);
	City* city_end = find_city_by_name(target);

	/* truck that will transport to city_end, todo make sure to add this guy to city_end's garage */
	CargoTruck truck = city_start->dequeue_cargo_truck();
	city_start->load_packages_to_truck(truck, start_package_count);

	/* now we are on midway station, load packages from there */
	city_mid->load_packages_to_truck(truck, midway_package_count);

	/* unload packages for midway station */
	for(size_t i = 0; i < midway_drop_indexes.size(); i++)
	{
		// new_idx = old_idx - num(idx's_deleted_before_old_idx that are lower than old_idx)
		auto old_idx = midway_drop_indexes[i];
		auto idx = old_idx - (int)std::count_if(midway_drop_indexes.begin(), midway_drop_indexes.begin() + i,
			[&](int el) {return el < old_idx; });
		city_mid->push_cargo_package(truck.pop_package_by_idx(idx));
	}

	/* we reached final city! unload all packages and park the truck */
	city_end->unload_packages_from_truck(truck);
	city_end->enqueue_cargo_truck(truck);
}

std::string CityContainer::get_output() const
{
	using std::stringstream;
	using std::endl;
	stringstream ret;

	/* copy everything, why? because we pop things to print them so actual state won't be modified this way. */
	DoublyLinkedList<City> copied_cities = cities;
	for (auto it = copied_cities.begin(); it != copied_cities.end(); ++it)
	{
		ret << it->get_name() << endl;
		
		ret << "Packages:" << endl;
		while(it->package_count())
		{
			ret << it->pop_cargo_package() << endl;
		}

		ret << "Trucks:" << endl;
		while(it->truck_count())
		{
			ret << it->dequeue_cargo_truck().get_name() << endl;
		}

		ret << "-------------" << endl;
	}

	return ret.str();
}

City* CityContainer::find_city_by_name(str_type city_name)
{
	City* match = cities.find([&](const City& c) {return c.get_name() == city_name; });
	if (!match)
	{
		std::cout << "[ERROR] No city with name " << city_name << " found!" << std::endl;
		// will crash if we reached here.
	}

	return match;
}
