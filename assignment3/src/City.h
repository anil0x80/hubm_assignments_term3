#pragma once
#include <string>
#include <utility>


#include "CargoTruck.h"
#include "Queue.h"
#include "Stack.h"

class City
{
public:
	City(std::string name) : city_name(std::move(name)){}

	void push_cargo_package(const std::string& package);
	std::string pop_cargo_package();
	
	void enqueue_cargo_truck(const CargoTruck& truck);
	CargoTruck dequeue_cargo_truck();

	std::string get_name() const;
	int truck_count() const;
	int package_count() const;

	/* loads package_count amount of packages from city into given truck */
	void load_packages_to_truck(CargoTruck& truck, int package_count);

	/* unloads all of the packages from truck and pushes them to city package */
	void unload_packages_from_truck(CargoTruck& truck);
	
private:
	std::string city_name;
	Stack<std::string> city_cargo_packages;
	Queue<CargoTruck> city_cargo_trucks;
};