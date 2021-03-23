#pragma once
#include <cassert>

template<typename T>
class Queue {
public:
	Queue() : _head(nullptr), _lastNode(nullptr), _size(0) // initialize vars
	{
		
	}
	~Queue()
	{
		clear();
	}

	/* copy constructor */
	Queue(const Queue& other) : Queue()
	{
		QueueNode* tmp = other._lastNode;
		for (int i = 0; i < other._size; i++)
		{
			enqueue(tmp->item);
			tmp = tmp->next;
		}
	}

	/* move constructor */
	Queue(Queue&& other) noexcept : Queue()
	{
		swap(*this, other);
	}

	/* assignment operator */
	Queue& operator=(Queue other) // if other is rvalue, move constructor is used, otherwise copy constructor is used
	{
		// other is locally copied and valid here.
		swap(*this, other);
		return *this;
	}

	/* swap */
	friend void swap(Queue& first, Queue& second) noexcept
	{
		using std::swap;
		swap(first._lastNode, second._lastNode);
		swap(first._head, second._head);
		swap(first._size, second._size);
	}

	void clear()
	{
		/* free memory */
		while (!isEmpty())
		{
			dequeue();
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
	
	//Enqueue: Items are added to the back of the queue
	void enqueue(const T& newItem)
	{
		/* assign a node to _head which has next as current head */
		auto* new_node = new QueueNode{ newItem, nullptr };
		if (!isEmpty())
		{
			/* if not empty, adjust link */
			_head->next = new_node;	
		}

		/* adjust head pointer */
		_head = new_node;
		
		_size++;

		if (_size == 1)
		{
			/* set _lastNode if _size is 1.*/
			_lastNode = _head;
		}
	}
	
	//Dequeue: Items are removed from the front of the queue
	void dequeue()
	{
		assert(_lastNode);
		_size--; // adjust size

		auto old_last_node = _lastNode;
		_lastNode = _lastNode->next; // adjust _lastNode
		delete old_last_node; // free the memory.
	}
	
	//Get Front: Take a look at the first item
	void getFront(T& queueTop) const
	{
		assert(_lastNode);
		
		// if list is not empty, _lastNode always points to the front 
		queueTop = _lastNode->item;
	}
	
private:
	struct QueueNode {
		T item;
		QueueNode* next;
	};
	
	QueueNode* _head; // append to here
	/* to avoid the traversal to the last node,
	 an additional pointer to the last node is defined*/
	QueueNode* _lastNode; // the node that will be popped first i.e, last added node.

	int _size;
};
