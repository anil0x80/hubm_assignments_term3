#include "GamePile.h"

std::string GamePile::get_card_display_at_line(int line_idx)
{
	/* line_idx is actually the index from bottom. */
	if (line_idx < this->get_card_count())
	{
		/* card will decide if it is open or not */
		return this->peek_bottom(line_idx)->encode();
	}
	else
	{
		/* no cards for this line */
		return std::string(3, ' ');
	}
}

bool GamePile::try_push_card(GameCard* card)
{
	if (!card->get_open())
	{
		/* can not push non-open cards to here. you will get cards from either foundation or waste or top of a pile, they are open. */
		return false;
	}

	/* if our pile is empty, the card must be the king (value == 13) */
	if (this->is_empty())
	{
		if (card->get_value() != 13)
		{
			/* trying to add non-king card into empty pile. won't work.. */
			return false;
		}
		else
		{
			this->push_card(card);
			return true;
		}
	}
	else
	{
		/* pile not empty, check if our top card is suitable with the new card */
		auto* top_card = this->peek_top(0);
		if (card->can_fit_below(top_card))
		{
			/* card can be added without a problem */
			this->push_card(card);
			return true;
		}
		else
		{
			/* card can not be added, probably its value or color is not suitable */
			return false;
		}
	}
}
