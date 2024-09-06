//Zeynep Taskin 
#include "Player.h"
#include <iostream>
using namespace std;


Player::Player(Board& aBoard,string name,int capital) :playerName(name),theBoard(aBoard), playerBalance(capital) {//constructor of the player class
	playerPosition = aBoard.reachFirst();//declare the position of th eplayer as the start of the board

};

void Player::display() {//displayer the player's position
	Node* temp = theBoard.reachFirst();//start from the starting slot
	while (temp != playerPosition) {//until player's position
		cout << "      ";
		temp = temp->next;//go to the next node
	}
	cout << playerName << " " << playerBalance;
	cout<<endl;
}

bool Player::move(int steps) {//cahnge the player's position "step" amount
	bool passStart = 0;
	for (int i = 0; i < steps; i++) {//until "step" amount is reached
		playerPosition = playerPosition->next;//change the player's position
		if (playerPosition == theBoard.reachFirst()) {//if player passes first
			passStart = 1;//make it true that player passes start
		}
	}
	return passStart;//return if player passes by first
};

Node* Player::where_am_I() {//return player's position
	return playerPosition;
}

int Player::get_balance() {//return player's balance
	return playerBalance;
}

void Player::deposit_money(int money) {//increase the player's money "money" amount
	playerBalance = playerBalance + money;
}

void Player::pay_to_player(Player& otherPlayer, int money) {//transfer money from one player to another
	playerBalance = playerBalance - money;//decrease player's money
	otherPlayer.deposit_money(money); //increase the another player's money
}

void Player::buy_slot(int slotMoney) {//buy the slot
	playerPosition->owner = playerName;//change the owner of the slot
	playerBalance = playerBalance - slotMoney;//decrease the player's balance
}

bool Player::is_bankrupt() {//return true if player has negative money
	if (playerBalance < 0) {
		return true;
	}return false;
}
