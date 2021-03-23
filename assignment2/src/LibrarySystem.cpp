#include "LibrarySystem.h"
#include <sstream>

LibrarySystem::LibrarySystem(const char* output_file_name) : output_file(output_file_name)
{
	output_file << "===Movie Library System===\n";
	output_file.flush();
}

void LibrarySystem::addMovie(const int movieId, const std::string movieTitle, const int year)
{
	if (last_operation != "addMovie")
	{
		output_file << "\n===addMovie() method test===" << std::endl;
	}
	last_operation = "addMovie";
	
	if (internal_get_movie_state(movieId) == not_in_db)
	{
		/* add movie to unchecked list */
		unchecked_movies_list.append({ movieId, movieTitle, year });
		output_file << "Movie " << movieId << " has been added" << std::endl;

	}
	else
	{
		/* movie exists in either a user's checked list or unchecked library list */
		output_file << "Movie " << movieId << " already exists" << std::endl;
		return;
	}
}

void LibrarySystem::deleteMovie(const int movieId)
{
	if (last_operation != "deleteMovie")
	{
		output_file << "\n===deleteMovie() method test===" << std::endl;
	}
	last_operation = "deleteMovie";
	
	auto should_delete = true;
	switch(internal_get_movie_state(movieId))
	{
	case checked:
		{
		output_file << "Movie " << movieId << " has been checked out" << std::endl;
		break;
		}
	case not_checked:
		{
		output_file << "Movie " << movieId << " has not been checked out" << std::endl;
		break;
		}
	case not_in_db:
		{
		output_file << "Movie " << movieId << " does not exist" << std::endl;
		should_delete = false;
		break;
		}
	}
	
	if (should_delete)
	{
		internal_delete_movie(movieId);
		output_file << "Movie " << movieId << " has been deleted" << std::endl;
	}

}

void LibrarySystem::addUser(const int userId, const std::string userName)
{
	if (last_operation != "addUser")
	{
		output_file << "\n===addUser() method test===" << std::endl;
	}
	last_operation = "addUser";

	auto* exists = user_list.find([&](User* user) {return user->id == userId; });
	if (exists)
	{
		/* user already exists */
		output_file << "User " << userId << " already exists" << std::endl;
		return;
	}
	else
	{
		/* can add */
		user_list.append({ userId, userName, {} });
		output_file << "User " << userId << " has been added" << std::endl;
	}
}

void LibrarySystem::deleteUser(const int userId)
{
	if (last_operation != "deleteUser")
	{
		output_file << "\n===deleteUser() method test===" << std::endl;
	}
	last_operation = "deleteUser";

	auto* exists = user_list.find([&](User* user) {return user->id == userId; });
	if (exists)
	{
		user_list.remove([&](User* user) {return user->id == userId; });
		output_file << "User " << userId << " has been deleted" << std::endl;
	}
	else
	{
		output_file << "User " << userId << " does not exist" << std::endl;
	}
}

void LibrarySystem::checkoutMovie(const int movieId, const int userId)
{
	if (last_operation != "checkoutMovie")
	{
		output_file << "\n===checkoutMovie() method test===" << std::endl;
	}
	last_operation = "checkoutMovie";
	
	auto* user = user_list.find([&](User* u) {return u->id == userId; });
	if (!user)
	{
		output_file << "User " << userId << " does not exist for checkout" << std::endl;
		return;
	}

	auto* movie = unchecked_movies_list.find([&](Movie* m) {return m->id == movieId; });
	if (!movie)
	{
		output_file << "Movie " << movieId << " does not exist for checkout" << std::endl;
		return;
	}

	user->checked_movies_list.append(*movie);
	
	unchecked_movies_list.remove([&](Movie* m) {return m->id == movieId; });
	output_file << "Movie " <<  movieId <<  " has been checked out by User "<<  userId << std::endl;
}

void LibrarySystem::returnMovie(const int movieId)
{
	if (last_operation != "returnMovie")
	{
		output_file << "\n===returnMovie() method test===" << std::endl;
	}
	last_operation = "returnMovie";

	switch (internal_get_movie_state(movieId))
	{
	case checked:
	{
		auto* owner = internal_get_user_by_movie_id(movieId);
		auto* movie = owner->checked_movies_list.find([&](Movie* m) {return m->id == movieId; });
		unchecked_movies_list.append(*movie);
		owner->checked_movies_list.remove([&](Movie* m) {return m->id == movieId; });
			
		output_file << "Movie " << movieId << " has been returned" << std::endl;
		break;
	}
	case not_checked:
	{
		output_file << "Movie " << movieId << " has not been checked out" << std::endl;
		break;
	}
	case not_in_db:
	{
		output_file << "Movie " << movieId << " not exist in the library" << std::endl;
		break;
	}
	}
	
}

void LibrarySystem::showAllMovies()
{
	if (last_operation != "showAllMovie")
	{
		output_file << "\n===showAllMovie() method test===" << std::endl;
		output_file << "Movie id - Movie name - Year - Status" << std::endl;
	}
	last_operation = "showAllMovie";

	/* print unchecked movies */
	output_file << internal_get_movie_list_display(nullptr, unchecked_movies_list);

	/* print checked movies */
	auto* cur_user = user_list.get_head();
	if (cur_user)
	{
		do
		{
			output_file << internal_get_movie_list_display(&cur_user->data, cur_user->data.checked_movies_list);
			
			cur_user = cur_user->next.get();
		} while (cur_user != user_list.get_head());
	}
}

void LibrarySystem::showMovie(const int movieId)
{
	if (last_operation != "showMovie")
	{
		output_file << "\n===showMovie() method test===" << std::endl;
	}
	last_operation = "showMovie";

	switch (internal_get_movie_state(movieId))
	{
	case checked:
	{
		auto* owner = internal_get_user_by_movie_id(movieId);
		auto* movie = owner->checked_movies_list.find([&](Movie* m) {return m->id == movieId; });

		output_file << internal_get_movie_display(owner, movie);
		break;
	}
	case not_checked:
	{
		auto* movie = unchecked_movies_list.find([&](Movie* m) {return m->id == movieId; });
		output_file << internal_get_movie_display(nullptr, movie);
		break;
	}
	case not_in_db:
	{
		output_file << "Movie with the id " << movieId << " does not exist" << std::endl;
		break;
	}
	}
	
}

void LibrarySystem::showUser(const int userId)
{
	if (last_operation != "showUser")
	{
		output_file << "\n===showUser() method test===" << std::endl;
	}
	last_operation = "showUser";

	auto* user = user_list.find([&](User* u) {return u->id == userId; });
	if (user)
	{
		/* print info */
		output_file << "User id: " << user->id << " User name: " << user->name << std::endl;
		output_file << "User " << user->id << " checked out the following Movies:" << std::endl;
		if (!user->checked_movies_list.empty())
		{
			output_file << "Movie id - Movie name - Year" << std::endl;
			output_file << internal_get_movie_list_display(nullptr, user->checked_movies_list, true);
		}
	}
	else
	{
		output_file << "User " << userId << " does not exist in the system" << std::endl;
	}
}


LibrarySystem::movie_state LibrarySystem::internal_get_movie_state(int movie_id)
{
	/* check if id already exists in any users */
	auto* user_has_movie = internal_get_user_by_movie_id(movie_id);
	
	/* check if id already exists in unchecked movie list */
	auto* movie_in_unchecked_list = this->unchecked_movies_list.find([&](Movie* movie) {return movie->id == movie_id; });

	if (user_has_movie)
		return checked;
	else if (movie_in_unchecked_list)
		return not_checked;

	return not_in_db;
}


void LibrarySystem::internal_delete_movie(int movie_id)
{
	auto* user = internal_get_user_by_movie_id(movie_id);
	if (user)
	{
		user->checked_movies_list.remove([&](Movie* movie) {return movie_id == movie->id; });
	}
	else
	{
		unchecked_movies_list.remove([&](Movie* movie) {return movie_id == movie->id; });
	}
	
}

User* LibrarySystem::internal_get_user_by_movie_id(int movie_id)
{
	auto* user_has_movie = user_list.find(
		[&](User* user) {return user->checked_movies_list.find(
			[&](Movie* movie) {return movie->id == movie_id; }); }
	);

	return user_has_movie;
} 

std::string LibrarySystem::internal_get_movie_display(User* user, Movie* movie, bool naked)
{
	std::stringstream ss;
	ss << movie->id << " " << movie->name << " " << movie->year;
	if (naked) 
	{
		ss << std::endl;
		return ss.str();
	}
	
	if (!user) ss << " Not checked out";
	else ss << " Checked out by User " << user->id;

	ss << std::endl;

	return ss.str();
}

std::string LibrarySystem::internal_get_movie_list_display(User* user, CircularSinglyLinkedList<Movie>& movie_list, bool naked)
{
	std::stringstream ss;
	auto* cur_movie = movie_list.get_head();
	if (cur_movie)
	{
		do
		{
			ss << internal_get_movie_display(user, &cur_movie->data, naked);
			cur_movie = cur_movie->next.get();
		} while (cur_movie != movie_list.get_head());
	}

	return ss.str();
}

void LibrarySystem::flush_output()
{
	output_file.flush();
}
