/*#include <iostream>
#include "DynQueue.h"
using namespace std;


DynQueue::DynQueue() {
	numOfPackets = 0;
	front = NULL;
	rear = NULL;

}

DynQueue::~DynQueue() {
	clear();  // Only clear if it hasn’t been manually cleared already

}

ostream& operator << (ostream& os, Packet& rhs) {
	os << rhs.id<<": ";
	for (int i = 0; i < rhs.routerHistory.size(); i++) {
		os << rhs.routerHistory[i];
		if (i != rhs.routerHistory.size() - 1) {
			os << ", ";
		}
	}
	return os;
}
ostream& operator << (ostream& os, DynQueue& rhs) {
	while (!rhs.isEmpty())
	{
		Packet node;
		rhs>>node;
		os<<node<<endl;
	}
	return os;
}

void DynQueue::operator << (Packet rhs) {
	if (isEmpty()) {   //if the queue is empty
		//make it the first element
		front = new QueueNode(rhs);
		rear = front;
	}
	else {  //if the queue is not empty
		//add it after rear
		rear->next = new QueueNode(rhs);
		rear = rear->next;
	}
	numOfPackets++;
}

void DynQueue::operator >> (Packet& rhs) {
	QueueNode* temp;
	if (isEmpty()) {
		cout << "Attempting to dequeue on empty queue, exiting program...\n";
		exit(1);
	}
	else {
		rhs = front->value;
		rhs.routerHistory = front->value.routerHistory;
		temp = front;
		front = front->next;
		delete temp;
		numOfPackets--;
	}
}



bool DynQueue::isEmpty(void) const {
	if (front == NULL)
		return true;
	else
		return false;
}


void DynQueue::clear(void) {
	Packet value;   // Dummy variable for dequeue
	while (!isEmpty())
		*this >> value; //delete all elements

}
*/