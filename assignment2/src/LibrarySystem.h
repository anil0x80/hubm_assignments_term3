#pragma once
#include "CircularDoublyLinkedList.h"
#include "CircularSinglyLinkedList.h"
#include "User.h"

#include <string>
#include <fstream>


class LibrarySystem
{
public:
	LibrarySystem(const char* output_file_name);
	
	void addMovie(const int movieId, const std::string movieTitle, const int year); 
	void deleteMovie(const int movieId);

	void addUser(const int userId, const std::string userName);
	void deleteUser(const int userId);

	void checkoutMovie(const int movieId, const int userId);
	void returnMovie(const int movieId);

	void showAllMovies();
	void showMovie(const int movieId);
	void showUser(const int userId);

	void flush_output();
private:
	enum movie_state
	{
		not_in_db,
		checked,
		not_checked
	};
	
	/* private functions */
	movie_state internal_get_movie_state(int movie_id);
	void internal_delete_movie(int movie_id);
	User* internal_get_user_by_movie_id(int movie_id);
	std::string internal_get_movie_display(User* user, Movie* movie, bool naked = false);
	std::string internal_get_movie_list_display(User* user, CircularSinglyLinkedList<Movie>& movie_list, bool naked = false);
	
	/* internal state*/
	// really, this is a bad way for storing them WHY don't we use map's for fast lookup by id? :/
	// i do not think using linked lists gives us any advantages for this program anymore...
	CircularDoublyLinkedList<User> user_list;
	CircularSinglyLinkedList<Movie> unchecked_movies_list;

	/* output */
	std::ofstream output_file;
	std::string last_operation;
};
