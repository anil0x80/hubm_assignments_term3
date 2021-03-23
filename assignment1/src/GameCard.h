#pragma once
#include <string>

enum CardColor : unsigned short
{
	Red,
	Black
};

enum CardType : unsigned short
{
	Hearts,
	Diamonds,
	Spades,
	Clubs
};

/* represents a single game card */
class GameCard
{
public:
	GameCard() = default;
	GameCard(CardType type, unsigned short value);

	std::string encode();

	bool get_open();
	void set_open(bool new_value);

	/* returns true if this object can form a correct building in a pile below the card 'root' */
	bool can_fit_below(GameCard* root);

	unsigned short get_value();
	CardType get_type();
	CardColor get_color();

private:
	bool is_open = false; // you have to keep track of this value.
	unsigned short value{}; // 1-13, value of the card.
	CardType type{};
};
