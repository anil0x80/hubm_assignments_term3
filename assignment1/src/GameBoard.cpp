#include "GameBoard.h"
#include <fstream>
#include <string>
#include <iostream>

#define IS_VALID_PILE(x) ((x < 7) && (x >= 0))
#define IS_VALID_FOUNDATION(x) ((x < 4) && (x >= 0))

/* sets up the game board */
GameBoard::GameBoard(const char* deck_file_name)
{
	/* read all lines from input file */
	auto index = 0;
	std::ifstream input(deck_file_name);
	for (std::string line; std::getline(input, line); )
	{
		/* sanity check */
		if (index >= 52)
		{
			std::cout << "More than 52 cards are detected on input file, program can not execute!" << std::endl;
			std::terminate();
		}

		/* each line represents a card, parse it */
		CardType card_type;
		switch (line[0])
		{
		case 'S':
			card_type = Spades;
			break;
		case 'D':
			card_type = Diamonds;
			break;
		case 'C':
			card_type = Clubs;
			break;
		case 'H':
			card_type = Hearts;
			break;
		default:
			/* unknown card tye */
			std::cout << "Unknown card type [" << line[0] << "] detected, program can not execute!" << std::endl;
			std::terminate();
		}

		const auto card_value = static_cast<unsigned short>(std::strtoul(&line[1], nullptr, 10));

		this->cards[index] = GameCard(card_type, card_value);

		index++;
	}

	/* setup the piles */
	for (auto i = 0; i < 7; i++)
	{
		/* math stuff */
		auto count = 0;
		auto offset = 0;
		for (auto k = i + 1; k > 0; k--)
		{
			this->piles[i].push_card(&this->cards[MAX_CARDS - 1 - i - offset]);
			offset += 6 - count;
			count++;
		}
	}

	/* make sure last cards on the pile are opened, so we can see stuff */
	for (auto& pile : this->piles)
	{
		pile.peek_top(0)->set_open(true);
	}

	/* setup the stock */
	for (auto i = 0; i < 24; i++) // 24 cards will be left for the stock
	{
		/* notice how we loop from 0 (BOTTOM) and append each card. */
		/* if we had looped from 23 (TOP), stock order would be upside down. */
		this->stock.push_card(&this->cards[i]);
	}
}

std::string GameBoard::get_board_as_string()
{
	std::string data = "\n"; // extra new line at start

	/* stock */
	if (!this->stock.is_empty())
		data += "@@@ ";
	else
		data += "___ ";

	/* waste */
	data += this->waste.get_as_string();

	data += std::string(9, ' ');

	/* foundations */
	for (auto& foundation : this->foundations)
	{
		// ref to this slot
		if (foundation.is_empty())
			data += "___";
		else
			data += foundation.peek_top(0)->encode();

		data += " ";
	}

	data += "\n\n"; // 2 new lines because why not

	/* piles */
	/* find the pile that contains maximum amount of cards */
	auto pile_maximum_card_count = 1;
	for (auto& pile : this->piles)
	{
		const auto card_count = pile.get_card_count();
		if (card_count > pile_maximum_card_count)
			pile_maximum_card_count = card_count;
	}
	
	for (auto i = 0; i < pile_maximum_card_count; i++)
	{
		/* print cards on piles for this line */
		for (auto& pile : this->piles)
		{
			data += pile.get_card_display_at_line(i);
			data += std::string(3, ' ');
		}
		data += '\n';
	}

	data += '\n'; // extra new line at end
	return data;
}

bool GameBoard::open_cards_from_stock()
{
	const auto stock_empty = this->stock.is_empty();
	const auto waste_empty = this->waste.is_empty();
	if (stock_empty && waste_empty)
	{
		/* nothing to do, this command is wrong for this scenario. */
		return false;
	}
	if (stock_empty)
	{
		/* reshuffle cards from waste into stock */
		while (auto* card = this->waste.pop_top())
		{
			card->set_open(false); // cards on stock are considered as not open.
			this->stock.push_card(card);
		}
	}
	else
	{
		/* transfer maximum 3 cards from stock into waste */
		const auto transfer_count = std::min(this->stock.get_card_count(), 3);
		for (auto i = 0; i < transfer_count; i++)
		{
			auto* card = this->stock.pop_top();
			card->set_open(true); // all cards on waste are considered as open.
			this->waste.push_card(card);
			/* adjust display count */
			this->waste.set_card_count_to_display(transfer_count); // show same amount of cards that we draw, if we
																// if we draw 2 cards, show 2 cards etc.
		}
	}

	return true;
}

bool GameBoard::move_to_foundation(int pile_idx)
{
	if (pile_idx != -1 && !IS_VALID_PILE(pile_idx))
	{
		/* invalid index was specified for pile_idx */
		return false;
	}
	auto* card = (pile_idx == -1) ? this->waste.peek_top(0) : this->piles[pile_idx].peek_top(0);
	
	if (!card || !card->get_open())
	{
		/* no card on specified place OR card is closed, wrong command */
		return false;
	}

	if (this->foundations[card->get_type()].try_push_card(card))
	{
		/* we have added the card to foundation successfully, now pop it from the source holder */
		if (pile_idx == -1) 
		{ 
			this->waste.pop_top(); 
			this->waste.set_card_count_to_display(this->waste.get_card_count_to_display() - 1);
		}
		else { this->piles[pile_idx].pop_top(); }
		return true;
	}
	else
	{
		/* invalid card to add to foundation.. */
		return false;
	}
	
}

bool GameBoard::move_from_pile_to_pile(int source_pile_idx, int source_card_idx, int destination_pile_idx)
{
	if (!IS_VALID_PILE(source_pile_idx) || !IS_VALID_PILE(destination_pile_idx))
	{
		/* one of the pile indexes are wrong */
		return false;
	}

	auto& source_pile = this->piles[source_pile_idx];
	auto& destination_pile = this->piles[destination_pile_idx];

	/* try to add each card, note that if top card is added, the others are guarenteed to be added */
	for (auto i = source_card_idx; i >= 0; i--)
	{
		auto* source_card = source_pile.peek_top(i);
		if (!source_card || !destination_pile.try_push_card(source_card))
		{
			/* card doesn't exist or not suitable */
			return false;
		}
	}

	/* do not forget to pop the cards from source pile */
	source_pile.pop_top_with_count(source_card_idx + 1);

	return true;
}

bool GameBoard::move_from_waste_to_pile(int destination_pile_idx)
{
	if (!IS_VALID_PILE(destination_pile_idx))
	{
		/* pile index is wrong */
		return false;
	}

	auto& destination_pile = this->piles[destination_pile_idx];

	auto* source_card = this->waste.peek_top(0);
	if (!source_card)
	{
		/* there are no cards on waste*/
		return false;
	}

	if (destination_pile.try_push_card(source_card))
	{
		/* successfully moved the card */
		this->waste.set_card_count_to_display(this->waste.get_card_count_to_display() - 1);
		this->waste.pop_top();

		return true;
	}
	else
	{
		/* card not suitable to push */
		return false;
	}
}

bool GameBoard::move_from_foundation_to_pile(int source_foundation_idx, int destination_pile_idx)
{
	if (!IS_VALID_PILE(destination_pile_idx) || !IS_VALID_FOUNDATION(source_foundation_idx))
	{
		/* some index is wrong */
		return false;
	}

	auto& destination_pile = this->piles[destination_pile_idx];
	auto& source_foundation = this->foundations[source_foundation_idx];

	auto* source_card = source_foundation.peek_top(0);
	if (!source_card)
	{
		/* there are no cards on specified foundation*/
		return false;
	}

	if (destination_pile.try_push_card(source_card))
	{
		/* successful move operation */
		source_foundation.pop_top();
		return true;
	}
	else
	{
		/* unsuitable card */
		return false;
	}
}

bool GameBoard::open(int pile_idx)
{
	if (!IS_VALID_PILE(pile_idx))
	{
		/* invalid pile idx */
		return false;
	}

	auto* card = this->piles[pile_idx].peek_top(0);
	if (!card || card->get_open())
	{
		/* if there is no card OR card is already open, return false */
		return false;
	}
	else
	{
		card->set_open(true);
		return true;
	}
}

void GameBoard::run_commands(const char* command_file_name, const char* output_file_name)
{
	/* variable to check if the game has ended */
	auto game_over = false;

	/* create output file */
	std::ofstream output_file(output_file_name);

	/* declare, lambdas that prevent code repetition */
	auto contains = [&](const std::string& line, std::string&& substring)
	{
		return line.find(substring) != std::string::npos;
	};
	auto get_int_at_idx = [&](const std::string& line, const int idx)
	{
		return std::stoi(line.substr(idx));
	};
	auto print_board = [&]()
	{
		output_file << this->get_board_as_string();
		output_file.flush();
	};

	/* print the board at the start of the game */
	print_board();

	/* read input file */
	std::ifstream command_file(command_file_name);
	for (std::string line; std::getline(command_file, line); )
	{
		/* print the command that we are executing */
		output_file << line << "\n\n";

		auto success = false;

		if (contains(line, "open from stock"))
		{
			success = this->open_cards_from_stock();
		}
		else if (contains(line, "move to foundation"))
		{
			/* check if we are moving from pile or waste */
			if (contains(line, "waste"))
			{
				/* move from waste*/
				success = this->move_to_foundation(-1);
			}
			else
			{
				/* move from pile */
				/*
				 * char idx 24 : source pile_idx
				 */
				const auto pile_index = get_int_at_idx(line, 24);
				success = this->move_to_foundation(pile_index);
			}
		}
		else if (contains(line, "move pile"))
		{
			/* move from pile to pile */
			/*
			 * char idx 10 : source pile idx
			 * char idx 12 : source card idx
			 * char idx 14 : destination pile idx
			 */
			success = this->move_from_pile_to_pile(get_int_at_idx(line,10),
				get_int_at_idx(line,12), get_int_at_idx(line, 14));
		}
		else if (contains(line, "move waste"))
		{
			/* move from waste to pile*/
			/*
			 * char idx 11 : destination pile idx
			 */
			success = this->move_from_waste_to_pile(get_int_at_idx(line, 11));
		}
		else if (contains(line, "move foundation"))
		{
			/* move from foundation to pile */
			/*
			 * char idx 16 : source foundation idx
			 * char idx 18 : destination pile idx
			 */
			success = this->move_from_foundation_to_pile(get_int_at_idx(line, 16), get_int_at_idx(line, 18));
		}
		else if (contains(line, "open"))
		{
			/* open the card that is on top of specified pile*/
			/*
			 * char idx 5 : pile idx
			 */
			success = this->open(get_int_at_idx(line, 5));
		}
		else if (contains(line, "exit"))
		{
			/* exit the game */
			game_over = true;
			success = true;
		}

		if (!success)
		{
			/* unsuccessful command */
			output_file << "Invalid Move!\n\n";
		}

		output_file << "****************************************\n";
		/* command ended, print the board */
		if (!game_over)
		{
			/* if exit is requested, we do not print the board */
			print_board();
		}
			

		/* check if user has won the game */
		auto game_won = true;
		for(auto& foundation: this->foundations)
		{
			if (foundation.get_card_count() < 13)
			{
				/* foundation is not full */
				game_won = false;
				break;
			}
		}
		if (game_won)
		{
			output_file << "****************************************\n\nYou Win!\n";
			game_over = true;
		}

		/* check if the game is over, either by exit command or user has won the game */
		if (game_over)
		{
			/* print game over and exit the loop */
			output_file << "\nGame Over!\n";
			break;
		}
	}

	output_file.close();
}
