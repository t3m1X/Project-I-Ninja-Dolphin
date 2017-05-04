#ifndef __PQUEUE_H__
#define __PQUEUE_H__

#define NULL 0

template <class T>
class pQueue {
private:
	struct q_item {
		q_item* next = nullptr;
		q_item* prev = nullptr;
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
		q_item* new_item = new q_item(item, priority);
		if (size == 0) {
			start = new_item;
			end = new_item;
		}
		else {
			q_item* tmp = end;
			while (tmp->priority > priority && tmp != start)
				tmp = tmp->prev;

			if (tmp != start || start->priority <= priority) {
				new_item->next = tmp->next;
				tmp->next = new_item;
				new_item->prev = tmp;
				if (tmp == end)
					end = new_item;
			}
			else {
				new_item->next = start;
				start->prev = new_item;
				start = new_item;
			}
		}
		++size;
	}

	void pop_front()
	{
		
		if (size == 0)
			return;

		/*T ret = start->data;*/
		q_item* tmp = start;

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

		return;
	}

	T pop_back()
	{
		if (size == 0)
			return NULL;

		T ret = end->data;
		q_item* tmp = end;

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

	T operator [] (int position) {
		q_item* ret = start;
		for (int i = 0; i < position && i < size; ++i) 
			ret = ret->next;

		return ret->data;
	}

	int Size() {
		return size;
	}
};

#endif