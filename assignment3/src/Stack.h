#pragma once
#include <cassert>

template <typename T>
class Stack {
public:

	Stack() : _head(nullptr), _size(0) // initialize vars
	{
		
	}
	~Stack()
	{
		clear();
	}

	/* copy constructor */
	Stack(const Stack& other) : Stack()
	{
		for (int j = other.size() - 1; j >= 0; j -= 1)
		{
			ListNode* tmp = other._head;
			for (int i = 0; i < j; i++)
			{
				tmp = tmp->next;
			}
			
			assert(tmp);
			push(tmp->item);
		}
	}

	/* move constructor */
	Stack(Stack&& other) noexcept : Stack()
	{
		swap(*this, other);
	}

	/* assignment operator */
	Stack& operator=(Stack other)
	{
		swap(*this, other);
		return *this;
	}

	friend void swap(Stack& first, Stack& second) noexcept
	{
		using std::swap;
		swap(first._head, second._head);
		swap(first._size, second._size);
	}
	
	void clear()
	{
		/* loop until there are no elements to pop */
		while (!isEmpty())
		{
			pop();
		}
	}
	
	bool isEmpty() const
	{
		return _size == 0;
	}
	
	int size() const
	{
		return _size;
	}

	//Push: Place item on top of the stack
	void push(const T& newItem)
	{
		/* modify head, also modify new node's next var to point to current _head*/
		_head = new ListNode{ newItem, _head };
		
		/* increase size */
		_size++;
	}
	
	//Top: Take a look at the topmost item without removing it
	void getTop(T& stackTop) const
	{
		assert(!isEmpty());
		stackTop = _head->item;
	}
	
	//Pop: Remove item from the top of the stack
	void pop()
	{
		assert(!isEmpty() && _head);
		_size--;

		auto* old_head = _head;
		_head = _head->next; // adjust head
		delete old_head; // free memory
	}
	
private:
	struct ListNode
	{
		T item;
		ListNode* next; // points to downwards
		// pops and pushes are O(1) == constant.
	};
	
	ListNode* _head;
	int _size;
};
