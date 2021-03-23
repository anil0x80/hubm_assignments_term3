#pragma once
#include "GameCardSlot.h"

// represents 1 pile slot of the piles areas, which has total 7 piles.
class GamePile : public GameCardSlot<20>
{
public:
	/* returns the representation of this pile on given index, note that answer might be all blanks */
	std::string get_card_display_at_line(int line_idx);

	/* tries to add the target card on top of this pile */
	bool try_push_card(GameCard* card);

private:

};