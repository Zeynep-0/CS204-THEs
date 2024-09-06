
#include <iostream>
#include<vector>
#include<string>
using namespace std;

//Author: Zeynep Taskin
//Date: July 2024 

int searchHori(string word, const vector<vector<string>>& matrix) {// search horizontally
	int lenofword = word.length();
	bool found = 0;

	if (matrix[0].size()<lenofword) {//check if the length of the word is longer than the row length
		return 0;
	}
	for (int i = 0; i < matrix.size(); i++) {//search each row
		for (int k = 0; k < matrix[i].size() - lenofword + 1; k++) {//set the starting index of the search
			string checkstr = "";// create a check string
			for (int j = k; j < lenofword + k; j++) {//get the same amount of letters as the word we search from the vector
				checkstr += matrix[i][j];
			}
			if (checkstr == word) {//compare the word and the str created
				found = true;
				break;//if they are the same break
			}
		}
	}
	if (found) {
		return lenofword;//if word is point get len amount of points
	}
	else {
		return 0;
	}
}
int searchVert(string word, const vector<vector<string>>& matrix) {//search vertically
	int lenofword = word.length();
	bool found = 0;

	if (matrix.size()<lenofword) {//check if the length of the word is longer than the column length
		return 0;
	}
	for (int i = 0; i < matrix[0].size(); i++) {//search each colmn
		for (int k = 0; k < matrix.size() - lenofword + 1; k++) {//set the starting index of the search
			string checkstr = "";// create a check string
			for (int j = k; j < lenofword + k; j++) {//get the same amount of letters as the word we search from the vector
				checkstr += matrix[j][i];
			}
			if (checkstr == word) {//compare the word and the str created
				found = true;
				break;//if they are the same break
			}
		}
	}

	if (found) {
		return lenofword;//if word is point get len amount of points
	}
	else {
		return 0;
	}
}

int searchDia(string word, const vector<vector<string>>& matrix) {//search diagnolly
	int lenofword = word.length();
	bool found = 0;
	if (matrix.size() < lenofword || matrix[0].size() < lenofword) {//check if the number of rows and columns are enough
		return 0;
	}
	
	for (int i = 0; i < matrix.size() - lenofword + 1; i++) {//determine the start row for search
		for (int j = 0; j < matrix[0].size() - lenofword + 1; j++) {//go over each possible start position in that row
			string checkstr = "";//create a string to compare
			for (int k = 0; k < lenofword; k++) {
				checkstr += matrix[i + k][j + k];//create the string
			}
			if (checkstr == word) {//compare the string to the word
				found = true;
				break;
			}
		}
	}
	
	if (found) {
		return lenofword * 2;//if word is found get len*2 amount of points
	}
	else {
		return 0;
	}
}
string reverseWord(string word) {//reverses the word
	string revword = "";
	for (int i = word.length() - 1; i != -1; i--) {//start from the last char and add one by one
		revword += word[i];
	}
	return revword;
}


int main() {
	int rowsNum, wordsNum, sum = 0, lastsum = 0;;
	string rows, word, words;
	vector<vector<string>>matrix;
	cin >> rowsNum;//get the row number

	for (int i = 0; i < rowsNum; i++) {//create the matrix
		vector<string>temp;
		cin >> rows;//get each line input
		for (int i = 0; i < rows.length(); i++) {
			temp.push_back(rows.substr(i, 1));//get each letter of the line and add it to temp
		}
		matrix.push_back(temp);//add temp vector to matrix
	}

	cin >> wordsNum;//get the word count

	for (int i = 0; i < wordsNum; i++) {
		cin >> word;//get the word
		sum = searchVert(word, matrix);
		sum += searchHori(word, matrix);
		sum += searchDia(word, matrix);
		//search in all directions
		if (sum == 0) {//if the word is not found
			words = reverseWord(word);//reverse the word and search again
			sum = searchVert(words, matrix);
			sum += searchHori(words, matrix);
			sum += searchDia(words, matrix);
		}
		if (sum != 0) {//if word is found
			if (word.length() > 5) {//if word is longer than 5 char
				lastsum += 2;//add 2 point
			}
		}
		else {//if word is not found decrease 5 point
			lastsum = lastsum - 5;
		}
		lastsum += sum;
	}
	cout << lastsum;
	return 0;
}
