
#include <iostream>
#include<fstream>
#include<string>
#include <vector>
using namespace std;

//Zeynep Taskin
const int ITEM_VAL = 204;
int score = 0;

struct MazeNode {//define the node structure
	char cellContent;//define node's content
	MazeNode* right, * left, * up, * down;//define node pointers for all directions
	MazeNode(char cell = 'A', MazeNode* r = nullptr, MazeNode* l = nullptr, MazeNode* u = nullptr, MazeNode* d = nullptr) // define the constructor
		: cellContent(cell), right(r), left(l), up(u), down(d) {}

};



MazeNode** _2DLinkedList(const vector<string>& mazeVector, int numRows, int numColumns) {//create and return 2D linked list of the maze
	string line;
	MazeNode** rowPtr = new MazeNode * [numRows];//create the pointer of pointer arrays
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

	return rowPtr;//return the created 2d linked list
}

void print2DLinkedList(MazeNode**& maze, int numRows, int numColumns) {//print the 2D linked list
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numColumns; j++) {
			cout << (maze[i] + j)->cellContent;//print each cell
		}
		cout << endl;
	}
}




void getTheX(MazeNode**& maze, int numRows, int numColumns, int& rowX, int& colX) {//find the X in the maze and update the rowX and colX
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



void deleteMaze(MazeNode**& maze, int numRows) {//deallocate the maze
	for (int i = 0; i < numRows; i++) {
		delete[] maze[i];//deallocate each array pointer
	}
	delete[] maze;//deallocate the pointer of array pointers
}

void itemCollectedCheck(bool& itemNotCollected, int keepRow, int keepCol, MazeNode**& maze) {//check wether the item is collected
	if (itemNotCollected && (maze[keepRow] + keepCol)->cellContent != 'X') {//if it is not collected and player is not on the item
		(maze[keepRow] + keepCol)->cellContent = 'I';//update the maze to show the item
	}
}

void getInput(char& key) {
	cout << "Enter input (R/L/U/D/P/C/Q):" << endl;
	cin >> key;
}


void pressedC(bool& itemNotCollected, char& key) {
	itemNotCollected = false;
	score += ITEM_VAL;
	cout << "Item collected! Your score is now " << score << "." << endl;
	getInput(key);
}


void moveTheX(bool& notEnd, string direction, MazeNode* ptr, char& key, int numRows, int numColumns, bool& itemNotCollected, int& keepRow, int& keepCol, int rowVal, int colVal, int& rowX, int& colX, MazeNode**& maze) {

	if (ptr == nullptr) {
		cout << "Cannot move: Out of the maze bound." << endl;
		getInput(key);
	}
	else if (ptr->cellContent == '#') {
		cout << "Cannot move: There is a wall in that direction." << endl;
		getInput(key);
	}
	else if (ptr->cellContent == 'I') {
		cout << "You moved " << direction << "." << endl;
		(maze[rowVal] + colVal)->cellContent = 'X';
		(maze[rowX] + colX)->cellContent = '.';
		cout << "You found an item worth "<<ITEM_VAL<<" points!" << endl;
		getInput(key);
		itemCollectedCheck(itemNotCollected, keepRow, keepCol, maze);
		getTheX(maze, numRows, numColumns, rowX, colX);
		if (key == 'C') {
			pressedC(itemNotCollected, key);
		}
		else {
			itemNotCollected = true;
			keepRow = rowVal;
			keepCol = colVal;
		}
	}
	else if (ptr->cellContent == 'E') {
		cout << "You moved " << direction << "." << endl;
		cout << "Congratulations! You reached the exit!" << endl;
		cout << "Final score: " << score;
		notEnd = false;
	}
	else {
		(maze[rowVal] + colVal)->cellContent = 'X';
		(maze[rowX] + colX)->cellContent = '.';
		getTheX(maze, numRows, numColumns, rowX, colX);
		cout << "You moved " << direction << "." << endl;
		getInput(key);
	}

}





int main() {
	ifstream file;
	string fileName, line;
	char key;
	cout << "Enter the maze file name:" << endl;
	cin >> fileName;//get the file name
	file.open(fileName.c_str());//open the file to read
	if (file.fail()) {//if file fails to open
		cout << "Unable to open file." << endl;
		cout << "Failed to load maze. Exiting...";
	}
	else {//if file does not fail to open
		cout << "Maze loaded. Start exploring!" << endl;
		getInput(key);//get the user input
		int numColumns = 0;
		int numRows = 0;
		vector<string> mazeVector;
		while (file >> line) {//count the rows and columns in the file
			numColumns = line.length();
			mazeVector.push_back(line);//push the data into a vector
			numRows += 1;
		}
		file.close();
		MazeNode** maze = _2DLinkedList(mazeVector, numRows, numColumns);//create the 2D linked list
		int rowX = 0, colX = 0, rowVal = 0, colVal = 0;
		bool itemNotCollected = false;//bool for checking if item is collected
		getTheX(maze, numRows, numColumns, rowX, colX);//get the location of x for keeping track of it
		int  keepRow = rowX, keepCol = colX;//define row and columns to keep for checking the items
		bool notEnd = true;
		while (notEnd) {//while game is not ended by reaching end
			switch (key) {
			case 'P'://if P is pressed
				itemCollectedCheck(itemNotCollected, keepRow, keepCol, maze);//check if there is an item that player passed by and not collected
				print2DLinkedList(maze, numRows, numColumns);//print the maze
				cout << "Current score: " << score << endl;//print the score
				getInput(key);
				break;

			case 'R'://if R is pressed
				rowVal = rowX;
				colVal = colX + 1;
				moveTheX(notEnd, "right", (maze[rowX] + colX)->right, key, numRows, numColumns, itemNotCollected, keepRow, keepCol, rowVal, colVal, rowX, colX, maze);
				break;

			case 'L':
				rowVal = rowX;
				colVal = colX - 1;
				moveTheX(notEnd, "left", (maze[rowX] + colX)->left, key, numRows, numColumns, itemNotCollected, keepRow, keepCol, rowVal, colVal, rowX, colX, maze);
				break;

			case 'U':
				rowVal = rowX - 1;
				colVal = colX;
				moveTheX(notEnd, "up", (maze[rowX] + colX)->up, key, numRows, numColumns, itemNotCollected, keepRow, keepCol, rowVal, colVal, rowX, colX, maze);
				break;

			case 'D':
				rowVal = rowX + 1;
				colVal = colX;
				moveTheX(notEnd, "down", (maze[rowX] + colX)->down, key, numRows, numColumns, itemNotCollected, keepRow, keepCol, rowVal, colVal, rowX, colX, maze);
				break;

			case 'C':
				if (itemNotCollected && (maze[keepRow] + keepCol)->cellContent == 'X') {//if C is pressed and there is an item where the player is
					pressedC(itemNotCollected, key);
				}
				else {//if there is not an item where the player is give appropriate message and ask for input again
					cout << "No item to collect here." << endl;
					getInput(key);
				}
				break;

			case 'Q'://when q is pressed end the game
				cout << "Game ended by the player." << endl;
				cout << "Final score: " << score;
				notEnd = false;
				break;

			default://if the input is invalid give error message and ask the input again
				cout << "Invalid input. Please try again." << endl;
				getInput(key);

			}
		}

		deleteMaze(maze, numRows);//when game ends deallocate the memory maze holds up
	}

	return 0;
}
