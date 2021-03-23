#include "City.h"

void City::push_cargo_package(const std::string& package)
{
	city_cargo_packages.push(package);
}

std::string City::pop_cargo_package()
{
	std::string popped;
	city_cargo_packages.getTop(popped);
	city_cargo_packages.pop();
	return popped;
}

void City::enqueue_cargo_truck(const CargoTruck& truck)
{
	city_cargo_trucks.enqueue(truck);
}

CargoTruck City::dequeue_cargo_truck()
{
	CargoTruck popped;
	city_cargo_trucks.getFront(popped);
	city_cargo_trucks.dequeue();
	return popped;
}

std::string City::get_name() const
{
	return city_name;
}

int City::truck_count() const
{
	return city_cargo_trucks.size();
}

int City::package_count() const
{
	return city_cargo_packages.size();
}

void City::load_packages_to_truck(CargoTruck& truck, int package_count)
{
	assert(package_count <= city_cargo_packages.size());
	
	for (auto i = 0; i < package_count; i++)
	{
		truck.append_package(pop_cargo_package());
	}
}

void City::unload_packages_from_truck(CargoTruck& truck)
{
	while(!truck.empty())
	{
		push_cargo_package(truck.pop_package());
	}
}
