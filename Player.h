//Zeynep Taskin 33851
#ifndef player_h
#define player_h
#include <iostream>
#include "Board.h"
using namespace std;

class Player {
private:
	string playerName;//player's name
	int playerBalance;//player's balance
	Board& theBoard;//board that can be shared
	Node* playerPosition;//player's position
public:
	Player(Board&,string,int);//constructor
	void display();//function to display the players position on the board
	bool move(int);//function to move the player and check if player passes by start of the board
	Node* where_am_I();//function to give th eplayer's position
	int get_balance();//function to getting the player's balance
	void deposit_money(int);//function to increase the player's balane
	void pay_to_player(Player&,int);//function to giving money to another player
	void buy_slot(int);//function for buying a slot on the board
	bool is_bankrupt();//function to check if the player bankrupted
};

#endif