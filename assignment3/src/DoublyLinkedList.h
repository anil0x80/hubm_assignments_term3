#pragma once
#include <cassert>
#include <utility>
#include <functional>

template<typename T>
class DoublyLinkedList
{
public:
	using predicate_type = std::function<bool(const T& item)>&&;
	
	DoublyLinkedList() : head(nullptr), tail(nullptr), size(0)
	{
		
	}
	
	~DoublyLinkedList()
	{
		clear();
	}

	/* copy constructor */
	DoublyLinkedList(const DoublyLinkedList& other) : DoublyLinkedList()
	{
		/* create this, by copying other */
		// loop other head to tail, call append on each item.
		Node* tmp = other.head;
		for(int i = 0; i < other.size; i++)
		{
			append(tmp->item);
			tmp = tmp->next;
		}
	}

	/* move constructor */
	DoublyLinkedList(DoublyLinkedList&& other) noexcept : DoublyLinkedList() // initialize via default constructor
	{
		swap(*this, other);
	}

	/* assignment operator */
	DoublyLinkedList& operator=(DoublyLinkedList other) // if other is rvalue, move constructor is used, otherwise copy constructor is used
	{
		swap(*this, other); // make copy of "other" and swap with "this"

		return *this;
	}

	/* swap, for copy and swap idiom */
	friend void swap(DoublyLinkedList& first, DoublyLinkedList& second) noexcept
	{
		using std::swap;

		/* swap members */
		swap(first.size, second.size);
		swap(first.head, second.head);
		swap(first.tail, second.tail);
	}

	int length() const
	{
		return size;
	}

	bool is_empty() const
	{
		return size == 0;
	}

	void clear()
	{
		while(!is_empty())
		{
			pop();
		}
	}

	/* search match against user predicate. Complexity is O(n) */
	T* find(predicate_type predicate)
	{
		Node* tmp = head;
		while(tmp) // loop head to tail
		{
			if (predicate(tmp->item))
				return &tmp->item;
			
			tmp = tmp->next;
		}
		return nullptr;
	}

	/* remove element at index(from head) and return it back*/
	T remove(int index) 
	{
		// check if index is correct
		assert(index >= 0 && index < size); // head is 0 and tail is size - 1

		/* handle these cases first. */
		if (index == 0)
			return pop();
		if (index == size - 1)
			return pop_tail();

		Node* target = nullptr; // node to be removed.

		/* determine target based on if it is closer to head or tail. decrease complexity to O(n/2) from O(n) */
		int index_from_tail = size - index - 1;
		if (index_from_tail < index)
		{
			/* looping from tail is more efficient, do it. */
			target = tail;
			for (auto i = 0; i < index_from_tail; i++)
			{
				target = target->prev;
			}
		}
		else
		{
			/* looping from head is more efficient, do it. */
			target = head;
			for (auto i = 0; i < index; i++)
			{
				target = target->next;
			}
		}

		assert(target);

		/* adjust links. we are sure target->next and target->prev is not nullptr,
		 * because we handled these cases at the start of this function (idx 0 and size-1)*/
		target->prev->next = target->next;
		target->next->prev = target->prev;
		T ret = std::move(target->item);
		delete target;
		size--;

		return ret;
	}

	/* append a new node to tail. */
	void append(const T& new_item)
	{
		tail = new Node{ new_item, nullptr, tail };
		
		size++;

		if (size > 1)
		{
			/* make sure to establish next link, if list has more than 1 element */
			tail->prev->next = tail;
		}
		
		if (size == 1)
			head = tail; // make sure head and tail points to same element if list has only 1 element
	}

	/* pop the element at index 0, i.e, from head.*/
	T pop()
	{
		assert(!is_empty() && head);

		auto* old_head = head;
		T ret = std::move(old_head->item); // make sure to not copy, since we already remove it.
		head = head->next;
		delete old_head;
		size--;

		return ret;
	}

	/* pop the element at index size - 1, i.e, from tail*/
	T pop_tail()
	{
		assert(!is_empty() && tail);

		auto old_tail = tail;
		T ret = std::move(old_tail->item); // make sure to not copy, since we already remove it.
		tail = tail->prev; // adjust tail
		delete old_tail;
		size--;

		return ret;
	}

private:
	struct Node
	{
		T item;
		Node* next;
		Node* prev;
	};

	Node* head;
	Node* tail;
	int size;

public:
	/* use this iterator to traverse through list */
	class ListIterator
	{
	public:
		ListIterator(Node* initial) : current(initial)
		{
			
		}

		// overload operator ++ for both postfix and prefix operators.
		ListIterator& operator++()
		{
			current = current->next;
			return *this;
		}
		ListIterator operator++(int)
		{
			ListIterator temp = *this;
			++*this;
			return temp;
		}
		
		// overload operator->
		T* operator->() const
		{
			return &current->item;
		}

		// overload equality and inequality operators
		bool operator==(const ListIterator& rhs) const
		{
			return current == rhs.current;
		}

		bool operator!=(const ListIterator& rhs) const
		{
			return current != rhs.current;
		}
		
	private:
		Node* current;
	};

	ListIterator begin()
	{
		return ListIterator(head); // start from head
	}

	ListIterator end()
	{
		return ListIterator(nullptr);
	}
};