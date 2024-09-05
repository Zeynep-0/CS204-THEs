//Zeynep Taskin 33851
#include "Board.h"
#include <iostream>
using namespace std;


Board::Board(int slots) {//constructor of the board
	first = new Node;
	Node* temp = first;
	for (int i = 0; i < slots - 1; i++) {//create new slots to the given slots argumnet
		temp->next = new Node;
		temp = temp->next;
	}
	temp->next = first;//declare last node's next to first node to create a circular linked list
}

Board::~Board() {//destructor of the board
	if (first->next == first) {// if the board only has the first node
		delete first;//delete the first node
	}
	else{
		Node* nextNode = first->next;
		Node* temp;
		while (nextNode != first) {//until the first node
			temp = nextNode;//get the slot
			nextNode = nextNode->next;//go to the next slot
			delete temp;//delete the slot
		}delete first;//delete first at last
	}
}

void Board::display() {//display the board
	Node* temp = first;
	while (temp->next != first) {//until node before first reached
		if (temp->owner != "None") {//if owner id not none
			cout << temp->owner << " " << "->";//print with a space after owner
		}
		else {
			cout << temp->owner<< "->";//print without a space after owner

		}
		temp = temp->next;//go to the next slot
	} 
	cout << temp->owner;//print the last node before first
	temp = temp->next;
	cout << endl;
	cout << "^     ";
	temp = temp->next;
	while (temp != first) {//until first
		if (temp->next == first)//if the node is one before first
			cout << "   |";
		else {
			cout << "      ";
		}
		temp = temp->next;//go to the next slot
	}
	cout << endl;
	cout << "|----<";
	temp = temp->next;
	while (temp != first) {//until first
		if (temp->next == first)//if the node is one before first
			cout << "---v";
		else {
			cout << "-----<";
		}
		temp = temp->next;//go to the next slot
	}
	cout << endl;
}


string Board::who_owns(Node* node) {//return who owns the slot
	return node->owner;
};


Node* Board::reachFirst() {//return the first node
	return first;
};
