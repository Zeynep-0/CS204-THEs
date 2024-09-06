//Zeynep Taskin 33851
#ifndef board_h
#define board_h
#include <iostream>
using namespace std;

struct Node {//node structor for slots on the board
	string owner;//owner of the slot
	Node* next;
	Node(string o = "None", Node* p = nullptr) :owner(o), next(p) {};
};
class Board {
private:
	Node* first;//starting slot of the board
public:
	Board(int);//constructor
	~Board();//destructor
	void display();//function to print the board
	string who_owns(Node*);//function to check who owns the specific slot
	Node* reachFirst();//returns the first node
};


#endif
