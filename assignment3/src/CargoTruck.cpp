#include "CargoTruck.h"

CargoTruck::CargoTruck() : truck_name(), truck_strength()
{
}

CargoTruck::CargoTruck(std::string name, float strength)
	: truck_name(std::move(name)), truck_strength(strength)
{
}

void CargoTruck::append_package(const std::string& package)
{
	truck_cargo_packages.append(package);
}

std::string CargoTruck::pop_package()
{
	return truck_cargo_packages.pop();
}

std::string CargoTruck::pop_package_by_idx(int idx)
{
	return truck_cargo_packages.remove(idx);
}

float CargoTruck::get_strength() const
{
	return truck_strength;
}

std::string CargoTruck::get_name() const
{
	return truck_name;
}

bool CargoTruck::empty() const
{
	return truck_cargo_packages.is_empty();
}
