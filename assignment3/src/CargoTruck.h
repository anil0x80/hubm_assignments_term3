#pragma once
#include <string>

#include "DoublyLinkedList.h"

class CargoTruck
{
public:
	CargoTruck();
	CargoTruck(std::string name, float strength);

	void append_package(const std::string& package);
	std::string pop_package();
	std::string pop_package_by_idx(int idx);
	
	float get_strength() const;
	std::string get_name() const;

	bool empty() const;
private:
	/* packages inside this truck*/
	DoublyLinkedList<std::string> truck_cargo_packages;

	/* name of the truck */
	std::string truck_name;
	
	/* only god knows what is this */
	float truck_strength;

};