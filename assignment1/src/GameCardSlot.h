#pragma once
#include "GameCard.h"

/* added to prevent code repetition. */
/* all of the implementation has to be done on header because this is a templated class. */
/* specific container that holds pointers to Cards of given size in an array, and exposes interfaces to interact with elements. */
template <int N>
class GameCardSlot
{
public:
	/* pushes the parameter card pointer into top of the cards, returns true if successful. */
	bool push_card(GameCard* card)
	{
		if (card_count < N)
		{
			cards[card_count] = card;
			card_count++;
			return true;
		}
		return false;
	}

	/* returns true if container is empty */
	bool is_empty()
	{
		return card_count == 0;
	}

	int get_card_count()
	{
		return card_count;
	}

	GameCard* pop_top()
	{
		if (is_empty())
		{
			/* nothing to pop */
			return nullptr;
		}

		card_count--;
		auto* temp = cards[card_count];
		cards[card_count] = nullptr;
		return temp;
	}

	/* removes n elements from the container */
	void pop_top_with_count(int n)
	{
		for (auto i = 0; i < n; i++)
		{
			this->pop_top();
		}
	}

	GameCard* peek_top(int idx)
	{
		idx = card_count - idx - 1; // convert top index to bottom index

		return peek_bottom(idx);
	}

	GameCard* peek_bottom(int idx)
	{
		if (idx < card_count && idx >= 0)
		{
			return cards[idx];
		}

		return nullptr;
	}

private:
	GameCard* cards[N]{};
	int card_count = 0;
};
