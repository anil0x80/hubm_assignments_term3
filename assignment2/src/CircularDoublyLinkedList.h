#pragma once
#include <memory>
#include <functional>

/* templated class definition must be inside a header file */
// needed improvements: iterators, pop method, remove_if method etc etc etc..................
template <typename T>
class CircularDoublyLinkedList
{
public:
	/* class definition for Node for  CircularDoublyLinkedList */
	struct Node
	{
		Node(T value) : data(value)
		{

		}

		T data;
		std::shared_ptr<Node> next;
		std::weak_ptr<Node> prev; // weak_ptr is useful when we hold reference to our owner.
	};
	
	using predicate_type = std::function<bool(T* item)>&&;  // accept rvalue reference, lambdas.

	 /* appends an element to the end of the list */
	void append(T data)
	{
		if (!tail)
		{
			/* list is empty, create head, assign tail to head */
			head = std::make_shared<Node>(data);
			tail = head;
		}
		else
		{
			/* list is not empty, tail is guaranteed to be valid */
			tail->next = std::make_shared<Node>(data); // create a new node with value and make tail point to it.
			tail->next->prev = tail;
			
		   /* update tail */
			tail = tail->next;
		}

		/* make sure tail->next points to head, because list is circular */
		tail->next = head;
		head->prev = tail; // make sure head's prev points to tail
	}
	
	/* expose head so user can iterate */
	Node* get_head()
	{
		return head.get();
	}

	Node* get_tail()
	{
		return tail.get();
	}
	
	/* search element by lambda, this allows to user to specify a condition to find specific element */
	T* find(predicate_type predicate)
	{
		Node* cur = head.get();
		if (cur)
		{
			do
			{
				if (predicate(&cur->data))
					return &cur->data; // return a pointer to data
				cur = cur->next.get();
			} while (cur != head.get()); // make sure we do not loop infinitely
		}
		
		return nullptr;
	}

	/* remove element by lambda, user can specify a custom filter by passing a lambda here. */
	void remove(predicate_type predicate)
	{
		/* remove first element that returns true for the given predicate */
		Node* cur = head.get();
		if (!cur) return; // list empty
		
		do
		{
			if (predicate(&cur->data)) // try to find the target node
			{
				if (cur == head.get())
				{
					/* the target element is the first element on the list */
					
					if(head.get() == tail.get()) // todo tidy
					{
						/* only 1 element is on list and it is time to remove it. */
						head = nullptr;
						tail = nullptr;
					}
					else
					{
						/* move head to right by 1 node */
						head = cur->next;
					}
				}
				else
				{
					if (cur == tail.get())
					{
						/* target element is the last element on the list, adjust tail */
						tail = cur->prev.lock(); // do not move here because we will use cur afterwards
					}
					// operator for shared ptr will free the deleted node's memory, when reference count reaches to 0.
					cur->prev.lock()->next = cur->next;
					cur->next->prev = cur->prev;
				}

				/* do not use cur after this point, it's next and prev pointers are transferred. */
				if (head && tail)
				{
					tail->next = head; // adjust tail to make sure it's circular
					head->prev = tail; // adjust prev to make sure it points to tail
				}
				
				return;
			}
			cur = cur->next.get();
			
		} while (cur != head.get()); // loop until we hit the head again
	}

private:
	/* private members of list */
	std::shared_ptr<Node> head; // first element of the list.
	std::shared_ptr<Node> tail; // last element of the list
};
