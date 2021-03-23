#include "GameFoundation.h"

bool GameFoundation::try_push_card(GameCard* card)
{
	/* type is not checked because it is guarenteed that the correct foundation with respect to card type will be called. */
	if (this->is_empty())
	{
		/* foundation is empty, card must have the value 1 */
		if (card->get_value() == 1)
		{
			/* correct card */
			this->push_card(card);
			return true;
		}
		else
		{
			/* trying to add a non-1 valued card to empty foundation, won't work */
			return false;
		}
	}
	else
	{
		/* foundation is not empty, card must have the value 1 more than the current top card */
		if (card->get_value() == this->peek_top(0)->get_value() + 1)
		{
			/* correct card */
			this->push_card(card);
			return true;
		}
		else
		{
			/* trying to add a card with wrong value, won't work */
			return false;
		}
	}
}
