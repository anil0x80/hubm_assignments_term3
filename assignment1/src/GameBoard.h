#pragma once
#include "GameWaste.h"
#include "GameStock.h"
#include "GamePile.h"
#include "GameFoundation.h"

#include <string>

constexpr auto MAX_CARDS = 52;

/* represents the internal game data */
class GameBoard
{
public:
	/* create a game instance from a file that contains cards */
	GameBoard(const char* deck_file_name);

	/* run commands from a file, and store the results in the output file */
	void run_commands(const char* command_file_name, const char* output_file_name);

private:
	/* gets the current representation of the board as string */
	std::string get_board_as_string();
	
	/* gameplay functions, their return value should NOT be discarded. */
	bool open_cards_from_stock();

	/* if pile_idx is specified as -1, the card from the wase is moved to the foundation instead. */
	bool move_to_foundation(int pile_idx);

	/* could have used overloading but this looks more clear imo */
	bool move_from_pile_to_pile(int source_pile_idx, int source_card_idx, int destination_pile_idx);
	bool move_from_waste_to_pile(int destination_pile_idx);
	bool move_from_foundation_to_pile(int source_foundation_idx, int destination_pile_idx);

	bool open(int pile_idx);

	/* all of the cards that are on board */
	GameCard cards[MAX_CARDS];

	/* components of the game, ALL of them are stored in ARRAY's. */
	GameStock stock;
	GameWaste waste;
	GameFoundation foundations[4]; // 0 -> hearth , 1 -> diamond, 2-> spade, 3 -> club
	GamePile piles[7]; // we have 7 piles in total.
};
