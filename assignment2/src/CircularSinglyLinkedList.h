#pragma once
#include <memory>
#include <functional>

/* templated class definition must be inside a header file */


template <typename T>
class CircularSinglyLinkedList
{
public:
	/* class definition for Node for CircularSinglyLinkedList */
	struct Node
	{
		Node(T value) : data(value)
		{

		}

		T data;
		std::shared_ptr<Node> next; // i would use unique_ptr if list wasn't circular.
	};
	
	using predicate_type = std::function<bool(T* item)>&&;  // accept rvalue reference, lambdas.

	bool empty()
	{
		return head == nullptr;
	}
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
		 	/* update tail */
			 tail = tail->next;
		 }

	 	/* make sure tail->next points to head, because list is circular */
		 tail->next = head;
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

	/* utility: search element by lambda, this allows to user to specify a condition to find specific element */
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

	/* utility: remove element by lambda, user can specify a custom filter by passing a lambda here. */
	void remove(predicate_type predicate)
	{
		std::shared_ptr<Node> prev_node = nullptr;
		std::shared_ptr<Node> cur_node = head;
		if (!cur_node) return; // list empty
	 	
		do 
		{
			if (predicate(&cur_node->data)) // try to find the target node
			{
				if (!prev_node)
				{
					/* remove the head, adjust stuff */
					
					if (head == tail)
					{
						/* only 1 element is on list and it is time to remove it. */
						head = nullptr;
						tail = nullptr;
					}
					else
					{
						head = cur_node->next; // adjust head, discard deleted element
					}
				}
				else
				{
					if (cur_node == tail)
					{
						/* target element is the last element on the list, adjust tail */
						tail = prev_node;
					}
					// operator for shared ptr will free the deleted node's memory, when reference count reaches to 0.
					prev_node->next = cur_node->next;
				}			

				if (head && tail)
					tail->next = head; // adjust tail to make sure it's circular

				return;
			}

			prev_node = cur_node;
			cur_node = cur_node->next;
			
		} while (cur_node != head); // loop until we hit the head again
	}
	
private:
	/* private members of list */
	std::shared_ptr<Node> head; // first element of the list.
	std::shared_ptr<Node> tail; // last element of the list
};
