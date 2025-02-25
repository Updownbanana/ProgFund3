#ifndef _STACK
#define _STACK

template<class ItemType>
class Stack {
public:
	ItemType peek();
	void pop();
	void push(ItemType& item);
	bool isEmpty();
};

#include "Stack.cpp"
#endif
