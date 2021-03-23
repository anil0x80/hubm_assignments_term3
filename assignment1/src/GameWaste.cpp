#include "GameWaste.h"

std::string GameWaste::get_as_string()
{
	std::string data;

	/* show one card from previous deck */
	if (this->card_count_to_display == 0 && !this->is_empty())
		this->card_count_to_display = 1;

	const auto waste_card_count_to_show = std::min(this->get_card_count(), this->card_count_to_display);
	// if the way for showing these are changed, just type in 3 instead of cards_to_show
	for (auto i = waste_card_count_to_show - 1; i >= 0; i--) // loop from last card to show to first card to shot
	{															// because text grows left to right..
		data += this->peek_top(i)->encode();
		if (i != 0 || waste_card_count_to_show != 3)
			data += " ";
	}
	for (auto i = 3 - waste_card_count_to_show; i > 0; i--)
	{
		data += "___";
		if (i != 1)
			data += " ";
	}

	return data;
}

int GameWaste::get_card_count_to_display()
{
	return this->card_count_to_display;
}

void GameWaste::set_card_count_to_display(int new_value)
{
	this->card_count_to_display = new_value;
}
