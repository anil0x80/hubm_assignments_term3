#include "GameCard.h"

/* assign member variables */
GameCard::GameCard(CardType type, unsigned short value) : type(type), value(value)
{
}

std::string GameCard::encode()
{
	if (!this->is_open)
	{
		/* card is not opened */
		return "@@@";
	}

	auto number_part = std::to_string(this->value);
	if (number_part.size() == 1)
	{
		/* if we have 1 digit, we need to pad it by appending 0 to front, i.e: 5 -> 05 */
		number_part = "0" + number_part;
	}

	std::string type_part;
	switch (this->type)
	{
	case Hearts:
		type_part = "H";
		break;
	case Diamonds:
		type_part = "D";
		break;
	case Spades:
		type_part = "S";
		break;
	case Clubs:
		type_part = "C";
		break;
	default:
		/* this can not fail because we already check it when we decode from file, so i left it unhandled.. */
		break;
	}

	return type_part + number_part;
}

bool GameCard::get_open()
{
	return this->is_open;
}

void GameCard::set_open(bool new_value)
{
	this->is_open = new_value;
}

bool GameCard::can_fit_below(GameCard* root)
{
	/* make sure none of the cards are closed */
	if (!this->is_open || !root->is_open)
		return false;

	/* colors should be opposite */
	if (this->get_color() == root->get_color())
		return false;

	/* this should have 1 less value than root */
	return this->value == root->value - 1;
}

unsigned short GameCard::get_value()
{
	return this->value;
}

CardType GameCard::get_type()
{
	return this->type;
}

CardColor GameCard::get_color()
{
	const auto type = this->get_type();
	return (type == Hearts || type == Diamonds) ? Red : Black;
}
