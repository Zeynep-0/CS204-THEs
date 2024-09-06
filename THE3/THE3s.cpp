
//Author: Zeynep Taskin
//Date: 15.08.2024 
//Purpose: This program simulates the distribution of packets across a network of routers.
//DynQueue.h
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

struct Packet {//structure that holds the necessary packet information 
	friend ostream& operator << (ostream&, Packet&);
	string id;//packet information
	vector<string> routerHistory;//vector of routers the packet visited
	Packet(string packetId = "abc", vector<string> history = vector<string>()) {//constructor
		id = packetId;
		routerHistory = history;
	}
	void addToHistory(string rout) {//adds the name of a visited router to the history
		routerHistory.push_back(rout);
	}
};

struct QueueNode {//structure representing a node in the queue
	Packet value;//packet it holds
	QueueNode* next;//pointer to the next node
	QueueNode(Packet& p, QueueNode* n = nullptr) {//constructor
		value = p;
		next = n;
	}
};

class DynQueue {//dynmaic queue class to store packets
	// give acces to private members to RouterNode class and output stream
	friend class RouterNode;
	friend ostream& operator << (ostream&, DynQueue&);
private:
	QueueNode* front;// pointer to the first node of the queue
	QueueNode* rear;// pointer to the last node of the queue
public:
	int numOfPackets;//number of packets in the queue
	DynQueue();//constructor
	~DynQueue();//destructor
	void operator << (Packet);//overloads "<<" operator to enqueue a packet
	void operator >> (Packet&);//overloads ">>" operator to dequeue a packet
	bool isEmpty(void) const;//checks if the queue is empty
	void clear(void);//clears the queue
};

//DynQueue.cpp

DynQueue::DynQueue() {//constructor that initializes the queue empty
	numOfPackets = 0;
	front = NULL;
	rear = NULL;

}

DynQueue::~DynQueue() {//destructor that clears the queue
	clear(); 

}

ostream& operator << (ostream& os, Packet& rhs) {//overloads the output stream operator to print the packet details
	os << rhs.id << ": ";
	for (int i = 0; i < rhs.routerHistory.size(); i++) {
		os << rhs.routerHistory[i];
		if (i != rhs.routerHistory.size() - 1) {
			os << ", ";
		}
	}
	return os;
}
ostream& operator << (ostream& os, DynQueue& rhs) {//overloads the output stream operator to print the queue
	while (!rhs.isEmpty())
	{
		Packet node;
		rhs >> node;//dequeue each packet
		os << node << endl;//print each packet
	}
	return os;
}

void DynQueue::operator << (Packet rhs) {//overloads "<<" operator to enqueue a packet into the queue
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
	numOfPackets++;//add to the number of packets of the queue
}

void DynQueue::operator >> (Packet& rhs) {//overloads ">>" operator to denqueue a packet into the queue
	QueueNode* temp;
	if (isEmpty()) {//check if the queue is empty
		cout << "Attempting to dequeue on empty queue, exiting program...\n";
		exit(1);
	}
	else {
		rhs = front->value;//copy the packet values on to rhs
		rhs.routerHistory = front->value.routerHistory;
		temp = front;//point to the front
		front = front->next;//update the front
		delete temp;//delete the previous front
		numOfPackets--;//decrease the number of packets of the queue
	}
}



bool DynQueue::isEmpty(void) const {//checks if the queue is empty
	if (front == NULL)//check if front pointer is null which means queue is empty
		return true;
	else
		return false;
}


void DynQueue::clear(void) {//clears the queue by dequeueing all packets
	Packet value;
	while (!isEmpty())
		*this >> value; //delete all elements
}


//main.cpp
class RouterNode {//class to manage router data and functionality

private:
	int nextNodeNum;//connected node number
	string name;//router name
	vector<string> nextNodes;//connected nodes 
public:
	DynQueue routerQue;//dynamic queue of the router
	//constructor
	RouterNode(string router = "", DynQueue routerQ = DynQueue(), vector<string> nextN = vector<string>()) :name(router), routerQue(routerQ), nextNodes(nextN) {
		nextNodeNum = nextNodes.size();
	}

	void addToQueue(Packet& node) {//adds a packet to the router's queue
		node.addToHistory(name);//add the routers name to the packets history
		routerQue << node;
	}

	string findRouterToSend(vector<RouterNode>& routerVec, int routerCount) {//finds the name of the next router to send the packet to
		string routerToSend = "a";//variable to hold the name of the next router
		int numOfPackets = -1;
		for (int j = 0; j < nextNodeNum; j++) {
			for (int i = 0; i < routerCount; i++) {
				if (nextNodes[j] == routerVec[i].name) {//if the next node is found
					//compare the packet numbers of the nodes, determine who has the least
					if (numOfPackets > routerVec[i].routerQue.numOfPackets || numOfPackets == -1) {
						numOfPackets = routerVec[i].routerQue.numOfPackets;
						routerToSend = routerVec[i].name;
					}
				}
			}
		}
		return routerToSend;// return the router name
	}

	void distributePackets(vector<RouterNode>& routerVec, int routerCount) {//distributes the packets in the queue
		while (!routerQue.isEmpty()) {//if there are packets
			string routerToSend = findRouterToSend(routerVec, routerCount);//finds name of the router to send to
			Packet temp;
			routerQue >> temp;//dequeue the packet
			for (int i = 0; i < routerCount; i++) {
				if (routerToSend == routerVec[i].name) {//find the router to send
					routerVec[i].addToQueue(temp);//add packet to the found router
				}
			}
		}
	}
};

vector<string> createConnectionsVec(string connectionStr) {//creates a vector that holds the names of the connected routers of a router
	vector<string> connectVec;//define the vector that holds the connections data
	int index = int(connectionStr.find(" "));//find the first pace in the connected router data
	while (index != string::npos) {//while a space can be found between connected router data

		string connection;
		connection += connectionStr.substr(0, index);//get the first connected router
		connectVec.push_back(connection);//push it to the vector
		connectionStr = connectionStr.substr(index + 1);//update the connected router data
		index = connectionStr.find(" ");//find the next space in the data
	}

	if (connectionStr.length() != 0) {//if there is still a connected router left 
		connectVec.push_back(connectionStr);//push it to the vector
	}
	return connectVec;//return the vector
}



int main() {
	string routerInput, packetInput;//variables to hold the input file names
	cout << "Please enter the name of the file for routers:" << endl;
	cin >> routerInput;//get the router input files name
	cout << "Please enter the name of the file for packets:" << endl;
	cin >> packetInput;//get the packets input file name
	cout << "Packets are displayed in the order they arrive in the terminal router, along with their router visit history:" << endl;

	int numOfRouters;//integer to hold the number of routers (excluding Entry and Terminal)
	ifstream routerFile, packetFile;//create input file streams for file inputs
	routerFile.open(routerInput);//open router file
	packetFile.open(packetInput);//open packet file

	routerFile >> numOfRouters;//read the number of routers
	string routerLine, packetLine;//variables to hold each line of input

	routerFile.ignore();

	vector<RouterNode> routerVec;//vector to hold the RouterNode obects keeping them in a sequential order

	for (int i = 0; i < numOfRouters + 1; i++) {
		getline(routerFile, routerLine);//get router input in a line
		int idx = int(routerLine.find(" - "));//parse the line to seperate router name and connected routers
		string name = routerLine.substr(0, idx);//extract the router name

		string connectionStr = routerLine.substr(idx + 3, routerLine.length() - idx);//extract the connected routers data
		vector<string> connectionVec = createConnectionsVec(connectionStr);//create a vector to hold the connected vector data

		DynQueue routerQueues;
		routerVec.push_back(RouterNode(name, routerQueues, connectionVec));//create the router node and push it to router vector

	}

	DynQueue routerTsQue;
	routerVec.push_back(RouterNode("T", routerTsQue, vector < string>()));//push the terminal router to the router vector

	while (!packetFile.eof()) {
		getline(packetFile, packetLine);//read a line from the packet file
		Packet packet(packetLine);
		routerVec[0].addToQueue(packet);//add the packet to the "Entry" queue
	}

	for (int i = 0; i < numOfRouters + 1; i++) {
		routerVec[i].distributePackets(routerVec, numOfRouters + 2);//distribute the packets to the routers
	}

	cout << routerVec[numOfRouters + 1].routerQue;//print the terminal routers queue's information

	return 0;
}*/





/*
#include <iostream>
#include <fstream>
#include <string>
#include "DynQueue.h"
#include<sstream>
using namespace std;

class RouterNode {
	friend class DynQueue;
private:
	int nextNodeNum;
	string name;
	vector<string> nextNodes;
public:
	DynQueue routerQue;
	RouterNode(string router = "", DynQueue routerQ = DynQueue(), vector<string> nextN = vector<string>()) :name(router), routerQue(routerQ), nextNodes(nextN) {
		nextNodeNum = nextNodes.size();
	}
	
	void addToQueue(Packet& node) {
		node.addToHistory(name);
		routerQue << node;

	}

	string findRouterToSend(vector<RouterNode>& routerVec, int routerCount) {
		string routerToSend="a";
		int numOfPackets = -1;
		for (int j = 0; j < nextNodeNum; j++) {
			for (int i = 0; i < routerCount; i++) {
				if (nextNodes[j] == routerVec[i].name) {
					if (numOfPackets > routerVec[i].routerQue.numOfPackets || numOfPackets == -1) {
						numOfPackets = routerVec[i].routerQue.numOfPackets;
						routerToSend = routerVec[i].name;
					}
				}
			}
		}
		return routerToSend;
	}

	void distributePackets(vector<RouterNode>& routerVec, int routerCount) {
		while (!routerQue.isEmpty()) {
			string routerToSend = findRouterToSend(routerVec, routerCount);
			Packet temp;
			routerQue >> temp;
			for (int i = 0; i < routerCount; i++) {
				if (routerToSend == routerVec[i].name) {
					routerVec[i].addToQueue(temp);
				}
			}
		}
	}
};

vector<string> createConnectionsVec(string connectionStr) {
	vector<string> connectVec;
	int index = int(connectionStr.find(" "));
	while (index != string::npos) {

		string connection;
		connection += connectionStr.substr(0, index);
		connectVec.push_back(connection);
		connectionStr = connectionStr.substr(index + 1);
		index = connectionStr.find(" ");
	}

	if (connectionStr.length() != 0) {
		connectVec.push_back(connectionStr);
	}
	return connectVec;
}



int main() {
	string routerInput, packetInput;
	cout << "Please enter the name of the file for routers:"<<endl;
	cin>>routerInput;
	cout << "Please enter the name of the file for packets:"<<endl;
	cin >> packetInput;
	cout << "Packets are displayed in the order they arrive in the terminal router, along with their router visit history:"<<endl;
	
	int numOfRouters;
	ifstream routerFile,packetFile;
	routerFile.open(routerInput);
	packetFile.open(packetInput);

	routerFile >> numOfRouters;
	string routerLine, packetLine;

	routerFile.ignore();

	vector<RouterNode> routerVec;

	for (int i = 0; i < numOfRouters + 1; i++) {
		getline(routerFile, routerLine);
		int idx = int(routerLine.find(" - "));
		string name = routerLine.substr(0, idx);

		string connectionStr = routerLine.substr(idx + 3, routerLine.length() - idx);
		vector<string> connectionVec = createConnectionsVec(connectionStr);
				
		DynQueue routerQueues;
		routerVec.push_back(RouterNode(name, routerQueues, connectionVec));
				
	}

	DynQueue routerTsQue;
	routerVec.push_back(RouterNode("T", routerTsQue, vector < string>()));

	while (!packetFile.eof()) {
			getline(packetFile, packetLine);
			Packet packet (packetLine);
			routerVec[0].addToQueue(packet);
	}

	for (int i = 0; i < numOfRouters+1; i++) {
		routerVec[i].distributePackets(routerVec,numOfRouters+2);//distribute the packets
	}

	cout << routerVec[numOfRouters+1].routerQue;
	
	return 0;
}
