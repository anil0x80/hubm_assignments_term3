#pragma once
#include "CircularSinglyLinkedList.h"
#include "Movie.h"

#include <string>

struct User
{
	int id;
	std::string name;
	CircularSinglyLinkedList<Movie> checked_movies_list;
};
