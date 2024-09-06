
//Author: Zeynep Taskin 
//Date: 31.07.2024 
//Purpose: This program is a maze game where you try to find the exit and collect items.

#include <iostream>
#include<fstream>
#include<string>
#include <vector>
using namespace std;


struct MazeNode {//defines the node structure
	char cellContent;//defines node's content
	MazeNode* right, * left, * up, * down;//defines node pointers for all directions
	MazeNode(char cell = 'A', MazeNode* r = nullptr, MazeNode* l = nullptr, MazeNode* u = nullptr, MazeNode* d = nullptr) // define the constructor
		: cellContent(cell), right(r), left(l), up(u), down(d) {}

};

class Maze {//create a class that creates the maze and the game
	public:
		Maze(void);//constructor of the maze
		void playGame(void);//member function that implements the game logic and allows user to play the game
		~Maze(void);//destructor of the maze

	private:
		const int ITEM_VAL = 204;//one item's value
		int score = 0;//total score
		char key = 'A';//user input
		vector<string> mazeVector;//vector that holds the values read from file input
		int numColumns = 0;//number of columns in the maze
		int numRows = 0;//number of rows in the maze
		MazeNode** maze = nullptr;//maze itself
		int rowX = 0;//X's row
		int colX = 0;//X's column
		int rowVal = 0;//row that player wants to go
		int colVal = 0;//column that player wants to go
		bool itemNotCollected = false;//bool for checking if item is collected
		int keepRow = 0;//row of the uncollected item
		int keepCol = 0;//column of the uncollected item
		bool notEnd = true;//bool for checking if the program ended
	
		void getInput(void);
		MazeNode** _2DLinkedList(void);
		void getTheX(void);
		void print2DLinkedList(void);
		void moveTheX(const string&, MazeNode*);
		void foundI(void);
		void itemCollectedCheck(void);
		void pressedC(void);
	
};


Maze::Maze() {//constructor that creates the maze
	ifstream file;
	string fileName, line;
	cout << "Enter the maze file name:" << endl;
	cin >> fileName;//get the file name
	file.open(fileName.c_str());//open the file to read

	if (file.fail()) {//if file fails to open
		cout << "Unable to open file." << endl;
		cout << "Failed to load maze. Exiting...";
		notEnd = false;
	}
	else {//if file does not fail to open
		cout << "Maze loaded. Start exploring!" << endl;
		getInput();//get the first user input
		while (file >> line) {//count the number of rows and columns in the file
			numColumns = int(line.length());
			mazeVector.push_back(line);//push the data into a vector
			numRows += 1;
		}
		file.close();//close the file
		maze = _2DLinkedList();//copy values from the vector to 2D linked list to create the maze
	}
}

void Maze::playGame() {//creates the game 
	getTheX();//get the location of x for keeping track of it
	while (notEnd) {//while game is not ended
		switch (key) {
		case 'P'://if P is pressed
			itemCollectedCheck();//check if there is an item that player passed by and not collected
			print2DLinkedList();//print the maze
			cout << "Current score: " << score << endl;//print the score
			getInput();//get the user input
			break;

		case 'R'://if R is pressed
			rowVal = rowX;//get the position of the right node
			colVal = colX + 1;
			moveTheX("right", (maze[rowX] + colX)->right);//move the X if possible
			break;

		case 'L'://if L is pressed
			rowVal = rowX;//get the position of the left node
			colVal = colX - 1;
			moveTheX("left", (maze[rowX] + colX)->left);//move the X if possible
			break;

		case 'U'://if U is pressed
			rowVal = rowX - 1;//get the position of the up node
			colVal = colX;
			moveTheX("up", (maze[rowX] + colX)->up);//move the X if possible
			break;

		case 'D'://if D is pressed
			rowVal = rowX + 1;//get the position of the down node
			colVal = colX;
			moveTheX("down", (maze[rowX] + colX)->down);//move the X if possible
			break;

		case 'C'://if C is pressed
			if (itemNotCollected && (maze[keepRow] + keepCol)->cellContent == 'X') {//if C is pressed and there is an item where the player is
				pressedC();//get the item
			}
			else {//if there is not an item where the player is give appropriate message and ask for input again
				cout << "No item to collect here." << endl;
				getInput();
			}
			break;

		case 'Q'://when Q is pressed end the game
			cout << "Game ended by the player." << endl;
			cout << "Final score: " << score;
			notEnd = false;
			break;

		default://if the input is invalid give error message and ask the input again
			cout << "Invalid input. Please try again." << endl;
			getInput();

		}
	}
}

Maze::~Maze() {//deallocate the maze
	for (int i = 0; i < numRows; i++) {
		delete[] maze[i];//deallocate each array pointer
	}
	delete[] maze;//deallocate the pointer of array pointers

}

void Maze::getInput() {//get user input
	cout << "Enter input (R/L/U/D/P/C/Q):" << endl;
	cin >> key;
}

MazeNode** Maze::_2DLinkedList() {//create and return 2D linked list of the maze
	string line;
	MazeNode** rowPtr = new MazeNode * [numRows];//create the pointer of pointer arrays to hold row pointers
	int row = 0;//go over each row
	for (int i = 0; i < mazeVector.size(); i++) {
		if (numRows != row) {

			line = mazeVector[i];//get each line from the vector
			MazeNode* ptrTemp = new MazeNode[numColumns];//create the temporary pointer array

			if (line.length() > 1) {//if there is more than one column
				ptrTemp[0] = MazeNode(line[0], &ptrTemp[1]);//set first node of the temp pointer and its right pointer

				for (int j = 1; j < numColumns - 1; j++) {
					ptrTemp[j] = MazeNode(line[j], &ptrTemp[j + 1], &ptrTemp[j - 1]);//set other nodes and their left and right pointer
				}
				ptrTemp[numColumns - 1] = MazeNode(line[numColumns - 1], nullptr, &ptrTemp[numColumns - 2]);//set the last node and its left pointer

			}
			else {//if there is only one column
				ptrTemp[0] = MazeNode(line[0]);//set the first node of the temp pointer
			}
			rowPtr[row] = ptrTemp;//assign each temp pointer to each row of the pointer of pointer array
			row++;//update the row
		}
	}

	for (int i = 1; i < numRows; i++) {
		for (int j = 0; j < numColumns; j++) {
			(rowPtr[i] + j)->up = (rowPtr[i - 1] + j);//update the up pointers of the nodes
		}
	}

	for (int i = 0; i < numRows - 1; i++) {
		for (int j = 0; j < numColumns; j++) {
			(rowPtr[i] + j)->down = (rowPtr[i + 1] + j);//update the down pointers of the nodes
		}
	}
	
	return rowPtr;//return the created 2D linked list
}

void Maze::getTheX() {//find the X(user's location) in the maze and update the rowX and colX
	char ch = 'X';
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numColumns; j++) {
			if ((maze[i] + j)->cellContent == ch) {
				rowX = i;
				colX = j;
			}
		}
	}
}

void Maze::print2DLinkedList() {//print the 2D linked list
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numColumns; j++) {
			cout << (maze[i] + j)->cellContent;//print each cell
		}
		cout << endl;
	}
}

void Maze::moveTheX(const string &direction, MazeNode* ptr) {//moves the X if possible

	if (ptr == nullptr) {//if there is no node in the next position
		cout << "Cannot move: Out of the maze bound." << endl;
		getInput();
	}
	else if (ptr->cellContent == '#') {//if there is a wall in the next position
		cout << "Cannot move: There is a wall in that direction." << endl;
		getInput();
	}
	else if (ptr->cellContent == 'I') {//if there is an item in the next position
		cout << "You moved " << direction << "." << endl;
		foundI();
	}
	else if (ptr->cellContent == 'E') {//if exit is the next position
		cout << "You moved " << direction << "." << endl;
		cout << "Congratulations! You reached the exit!" << endl;
		cout << "Final score: " << score;
		notEnd = false;//end the game
	}
	else {//if there is no special case 
		(maze[rowVal] + colVal)->cellContent = 'X';//move the X
		(maze[rowX] + colX)->cellContent = '.';//update the previous position
		getTheX();//update the X's position
		cout << "You moved " << direction << "." << endl;
		getInput();
	}

}

void Maze::foundI() {//if I is found in the next position
	(maze[rowVal] + colVal)->cellContent = 'X';//change the X's position
	(maze[rowX] + colX)->cellContent = '.';
	cout << "You found an item worth " << ITEM_VAL << " points!" << endl;
	getInput();
	itemCollectedCheck();//check if the previous position had an Item
	getTheX();//update X's position
	if (key == 'C') {//if C pressed
		pressedC();
	}
	else {//if item is not collected
		itemNotCollected = true;//update the item checking bool
		keepRow = rowVal;//keep the positions of the Item
		keepCol = colVal;
	}
}

void Maze::itemCollectedCheck() {//check wether the item is collected
	if (itemNotCollected && (maze[keepRow] + keepCol)->cellContent != 'X') {//if it is not collected and player is not on the item
		(maze[keepRow] + keepCol)->cellContent = 'I';//update the maze to show the item
	}
}

void Maze::pressedC() {//if C is pressed 
	itemNotCollected = false;//item is collected
	score += ITEM_VAL;//add value to the score
	cout << "Item collected! Your score is now " << score << "." << endl;//print score
	getInput();
}


int main() {
	Maze maze;//create the maze
	maze.playGame();//play the game
	return 0;
}
