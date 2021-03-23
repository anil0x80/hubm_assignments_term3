#pragma once
#include "GameCardSlot.h"

/* represents the game waste. */
class GameWaste : public GameCardSlot<24>
{
public:
	std::string get_as_string();

	int get_card_count_to_display();
	void set_card_count_to_display(int new_value);
private:
	int card_count_to_display = 0; // 0-3. If we have a open from stock command, set this to 3 so it shows top 3 cards
					 // then each time a card is moved from stock, decrease this number by 1.
					// if this number reaches 0 and the card holder is empty, do not show anything
					// if this number reaches 0 and there are cards to show, increase the number by 1 and show only 1 card from the top.
					// NOTE THAT this number is only for display, it has no serve on internal game play.
};