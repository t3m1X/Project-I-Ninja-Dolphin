#ifndef __PQUEUE_H__
#define __PQUEUE_H__

#define NULL 0

template <class T>
class pQueue {
private:
	struct q_item {
		q_item* next;
		q_item* prev;
		unsigned int priority;

		T data;

		q_item (const T& data, const unsigned int& priority) : data(data), priority(priority)
		{}
	};

	q_item* start;
	q_item* end;
	unsigned int size;

public:

	pQueue() : start(nullptr), end(nullptr), size(0) 
	{}

	void push_back(const T& item, const unsigned int& priority)
	{
		q_item new_item = new q_item(item, priority);
		if (size == 0) {
			start = new_item;
			end = new_item;
		}
		else {
			q_item tmp = end;
			while (tmp->priority > priority && tmp != start)
				tmp = end->prev;

			tmp->prev->next = new_item;
			new_item->prev = tmp->prev;
			new_item->next = tmp;
			tmp->prev = new_item;
		}
		++size;
	}

	T pop_front()
	{
		
		if (size == 0)
			return NULL;

		T ret = start->data;
		q_item tmp = start;

		if (size > 1) {
			start->next->prev = nullptr;
			start = start->next;
		}
		else {
			start = nullptr;
			end = nullptr;
		}
		
		delete tmp;
		--size;

		return ret;
	}

	T pop_back()
	{
		if (size == 0)
			return NULL;

		T ret = end->data;
		q_item tmp = end;

		if (size > 1) {
			end->prev->next = nullptr;
			end = end->prev;
		}
		else {
			start = nullptr;
			end = nullptr;
		}

		delete tmp;
		--size;

		return ret;
	}
};

#endif