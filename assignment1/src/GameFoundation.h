#pragma once
#include "GameCardSlot.h"

/* represents one slot of the foundation area, which has 4 slots in total */
class GameFoundation : public GameCardSlot<13>
{
public:
	/* adds if the specified card is valid to add to this foundation, else returns false. */
	bool try_push_card(GameCard* card);

private:

};
