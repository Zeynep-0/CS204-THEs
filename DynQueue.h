/*
#ifndef DYNQUEUE_H
#define DYNQUEUE_H

#include <iostream>
#include<vector>
using namespace std;

struct Packet {
	friend ostream& operator << (ostream&, Packet&);
	string id;
	vector<string> routerHistory;
	Packet(string idof="abc", vector<string> history= vector<string>()) {
		id = idof;
		routerHistory = history;
	}
	void addToHistory(string rout) {
		routerHistory.push_back(rout);
	}
};

struct QueueNode {
	Packet value;
	QueueNode* next;
	QueueNode(Packet& p, QueueNode* n=nullptr) {
		value = p;
		next = n;
	}
};



class DynQueue {
friend class RouterNode;
friend ostream& operator << (ostream& , DynQueue&);
private:
	QueueNode* front;
	QueueNode* rear;
public:
	int numOfPackets;
	DynQueue();
	~DynQueue();
	void operator << (Packet);
	void operator >> (Packet&);
	bool isEmpty(void) const;
	void clear(void);
};


#endif // DYNQUEUE_H*/