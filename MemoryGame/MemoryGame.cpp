// MemoryGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <array>
#include <random>
#include <ctime>
#include <thread>
#include <string>
using namespace std;

const size_t numRows{ 4 };
const size_t numColumns{ 4 };
const size_t totalCharacters{ 16 };
void shuffleCards(string (&letterCards)[16]);
void displayCards(const array<array<string, numRows>, numColumns>&);
bool getInputs(int&, int&, int&, int&);
void revealCards(int, int, int, int, const array<array<string, numRows>, numColumns>&, const array<array<string, numRows>, numColumns>&);
bool checkEquality(int, int, int, int, const array<array<string, numRows>, numColumns>&);
bool checkIfComplete(const array<array<string, numRows>, numColumns>&);
void hideCards();
void wonGame(int);

int main()
{

	bool quit{ false };
	bool complete{ false }; 
	int totalMoves{ 0 };
	int firstCardX{ 0 }, firstCardY{ 0 }, secondCardX{ 0 }, secondCardY{ 0 };

	//Create an initial array with all of the characters needed for the game. These will be randomized at a later time
	string letterCards[16] {"A", "C", "E", "G",
						  "A", "C", "E", "G",
						  "B", "D", "F", "H",
						  "B", "D", "F", "H" };

	//Create an array of "X" values to represent the cards face down.
	//Once the cards are correctly paired they will remain face up
	array<array<string, numRows>, numColumns> cards{ "X", "X", "X", "X",
												  "X", "X", "X", "X",
												  "X", "X", "X", "X",
												  "X", "X", "X", "X",
	};

	//Call shuffleCards function to randomize the values in the array
	shuffleCards(letterCards);

	//Assign the randomized letters into a 2D array
	array<array<string, numRows>, numColumns> shuffledCards{ letterCards[0], letterCards[1], letterCards[2], letterCards[3],
														  letterCards[4], letterCards[5], letterCards[6], letterCards[7],
														  letterCards[8], letterCards[9], letterCards[10], letterCards[11],
														  letterCards[12], letterCards[13], letterCards[14], letterCards[15],
	};


	quit = getInputs(firstCardX, firstCardY, secondCardX, secondCardY);

	//Loop to run the main body of the program.
	//If the user has not quit the game and also has not
	//completed the game they will be asked to select new cards.
	//These cards will be compared and will remain face up for 3s, unless 
	//they match, in which case they will be permentantly flipped up 
	while (!quit && !complete) {
		totalMoves++;
		
		revealCards(firstCardX, firstCardY, secondCardX, secondCardY, shuffledCards, cards);
		if (checkEquality(firstCardX, firstCardY, secondCardX, secondCardY, shuffledCards)) {
			cards[firstCardX][firstCardY] = shuffledCards[firstCardX][firstCardY];
			cards[secondCardX][secondCardY] = shuffledCards[secondCardX][secondCardY];

		}
		//Check if all the cards have been matched.
		//if so, the game will be completed.
		//If not, the user will be prompted to pick a new pair
		complete = checkIfComplete(cards);
		if (complete) {
			wonGame(totalMoves);
			system("pause");
			return 0;
		}
		displayCards(cards);
		getInputs(firstCardX, firstCardY, secondCardX, secondCardY);
		

	}

	//If the user quits the game after it is complete, let them know how many moves it took them
	if (complete && !quit) {
		cout << "Congratulations! You've solved the puzzle in " << totalMoves << " moves." << endl;
	}
	//If the user quits the game, let them know the game is over.
	else {
		cout << "You have quit the game." << endl;
	}

	system("pause");
	return 0;

}

//Use shuffle() to randomize the cards in the array using a timestamp seed
void shuffleCards(string(&letterCards)[16]) {
	
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	shuffle(begin(letterCards), end(letterCards), default_random_engine(seed));

}

//Display the cards that have been correctly identified alongside the face down cards
void displayCards(const array<array<string, numRows>, numColumns>& cards){
	for (unsigned int row = 0; row < numRows; row++) {
		for (unsigned int column = 0; column < numColumns; column++) {
			cout << cards[row][column] << ' ';
		}
		cout << endl;
	}
}

//Prompt the user to input the x and y location of two cards.
//If the user provides q, the game will end.
//Otherwise, the two cards will be assigned to variables and displayed in the main program
bool getInputs(int& firstCardX, int& firstCardY, int& secondCardX, int& secondCardY) {
	char firstCardXInput, firstCardYInput, secondCardXInput, secondCardYInput;
	cout << "\nSelect two cards (type \'q\' to quit the game): ";
	cin >> firstCardXInput;
	if (firstCardXInput == 'q') {
		return true;
	}
	else {
		cin >> firstCardYInput >> secondCardXInput >> secondCardYInput;
		firstCardX = firstCardXInput - '0';
		firstCardY = firstCardYInput - '0';
		secondCardX = secondCardXInput - '0';
		secondCardY = secondCardYInput - '0';
		return false;


	}
}

//Reveal cards based on the user's input from the getInputs() function.
//For the remaining cards, display either the value (if the card has been successfully matched)
//or 'X' if the value is unknown
void revealCards(int firstCardX, int firstCardY, int secondCardX, int secondCardY, const array<array<string, numRows>, numColumns>& shuffledCards, const array<array<string, numRows>, numColumns>& cards) {
	for (unsigned int row = 0; row < numRows; row++) {
		for (unsigned int column = 0; column < numColumns; column++) {
			if (row == firstCardX && column == firstCardY) {
				cout << shuffledCards[row][column] << ' ';
			}
			else if (row == secondCardX && column == secondCardY) {
				cout << shuffledCards[row][column] << ' ';
			}

			else {
				cout << cards[row][column] << ' ';

			}
			
		}
		cout << endl;
		
	}
	cout << endl;

	hideCards();
}

//Compare the value of the two selected cards.
//If they match, the cards will remain face up.
bool checkEquality(int firstCardX, int firstCardY, int secondCardX, int secondCardY, const array<array<string, numRows>, numColumns>& shuffledCards) {
	if (shuffledCards[firstCardX][firstCardY] == shuffledCards[secondCardX][secondCardY]) {
		return true;			
	}
	else {
		return false;
	}
}

//Scan the board to see if the game is completed.
//If no 'X' remains on the board, than the game has been won
bool checkIfComplete(const array<array<string, numRows>, numColumns>& cards) {
	bool result = true;
	for (unsigned int row = 0; row < numRows; row++) {
		for (unsigned int column = 0; column < numColumns; column++) {
			if (cards[row][column] == "X") {
				result = false;
			}
			
		}
	}
	return result;
}

//Display the cards for 3s and then clear the console window
void hideCards() {
	for (int i = 3; i > 0; i--)
	{
		this_thread::sleep_for(1s);
		cout << i << endl;
	}
	system("CLS");

}

//Once the user has won the game, let them know how many moves it took for them to solve
void wonGame(int totalMoves) {
	cout << "Congratulations! You've solved the puzzle in " << totalMoves << " moves." << endl;
}

