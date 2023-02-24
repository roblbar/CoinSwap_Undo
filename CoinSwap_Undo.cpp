/*
/ Robert Barrera
/ CS 141 at UIC
/ Program 6: COINSWAP UNDO
/
/ Retry of Program 6 in CS 141 in C++
*/

#include <iostream>		// For standard input output
#include <cstring>		// For string functions
#include <cctype>		// For character functions
#include <cstdlib>		// For random numbers
#include <cmath>		// For fabs function


using namespace std;
#pragma warning(disable:4996)

// Global constants
const int BoardSize = 5;

// Displays programmer's information
void displayProgrammerInfo() {
	cout << "Author: Robert L Barrera\n";
	cout << "Program: #6, CoinSwap Undo\n";
	cout << "TA: Forgot\n";
	cout << "Feb 01, 2023\n\n";
}


// Displays introduction
void displayIntroduction() {
	cout << "Welcome to the coin swap puzzle.\n";
	cout << "Make moves to solve the puzzle where the objective is to swap the \n";
	cout << "place of the pieces on either side of the board.  X can only move \n";
	cout << "to the right into an empty square, or can jump to the right over \n";
	cout << "an O into an empty square. Conversely O can only move to the left \n";
	cout << "into an empty square, or can jump to the left over an X into an \n";
	cout << "empty square. \n\n";
	cout << "For each move enter the source(1..5) and destination(1..5).\n";
	cout << "Enter x to exit the program.\n";
}


// Node struct for game move information
struct GameMove {
	char board[BoardSize + 1];  // Extra character for the NULL
	int moveNumber;				// Game move number
	GameMove *toLastMove;		// Pointer to last GameMove
};


// Reed's code
//--------------------------------------------------------------------------------
// See if board pieces have finished being swapped.  This is the case when
// the left-most two pieces are both 'O' and the right-most two pieces are 'X'
bool notDone(char board[])
{
	return board[0] != 'O' || board[1] != 'O' || board[3] != 'X' || board[4] != 'X';
}

// For displaying the list when 'l' is entered, for debugging
void displayList(GameMove *lastMove) {
	GameMove *tempMove = lastMove;	// Temp move for moving through list
	cout << "   List: ";

	while (tempMove != NULL) {	// Skip printing list if first node is null
		cout << tempMove->moveNumber << "[";	// Print mover number
		cout << tempMove->board;	// Print board
		cout << "] ";

		tempMove = tempMove->toLastMove;	// Move tempMove to next GameMove
	}

	cout << endl;
}


// Reed's code
//--------------------------------------------------------------------------------
void displayBoard(char board[], GameMove *lastMove)
{
	if (lastMove != nullptr) {			// If last move is not null
		strcpy(board, lastMove->board);	// Copy lastMove's board to main board
	}

	cout << endl;
	cout << "   1 2 3 4 5\n";
	cout << "   ";							// Print board
	for (int i = 0; i<BoardSize; i++) {
		cout << board[i] << " ";
	}

	GameMove *tempMove = lastMove;			// TempMove for moving through list
	cout << "   List: " << tempMove->moveNumber;	// Print GameMove
	while (tempMove->toLastMove != NULL) {
		cout << "->";
		tempMove = tempMove->toLastMove;	// Move tempMove to last GameMove
		cout << tempMove->moveNumber;		// Print previous GameMove
	}

	cout << endl << endl;
}


// Adds game move to linked list
void addGameMovetoList(char board[], int currentMoveNumber, GameMove *&lastMove) {
	GameMove *newMove = new GameMove;	// NewMove points to a new GameMove

	newMove->moveNumber = currentMoveNumber;	// Store current move number in newMove
	strcpy(newMove->board, board);		// Store current board in newMove
	newMove->toLastMove = lastMove;		// Point newMove to last move
	lastMove = newMove;					// Make newMove the last move
}


// Undo last move in list
void undoLastMove(GameMove *&lastMove, int &moveNumber) {
	GameMove *tempMove = lastMove;	// Points tempMove to last GameMove

	if (lastMove->toLastMove != NULL) {
		lastMove = lastMove->toLastMove;	// LastMove now points to the 2nd last move in list
		delete tempMove;		// Deletes the GameMove tempMove points to
		--moveNumber;			// Remove last move number
		cout << "* Undoing move *" << endl;
	}
}


// Reed's code
//--------------------------------------------------------------------------------
void promptForAndGetMove(char board[], int &moveNumber,
	int &source, int &destination, GameMove *&lastMoveMade)
{
	char userInput;

	// Infinite loop to handle possible multiple undo of moves
	while (true) {
		cout << moveNumber << ". Enter source and destination: ";
		cin >> userInput;
		// See if user input of x was given to exit the program
		if (userInput == 'x') {
			cout << "\n";
			cout << "Exiting program...\n";
			exit(0);
		}
		if (userInput == 'u') {
			// undo move
			// ..
			if (moveNumber == 1) {	// Do not undo past 1st move
				cout << "*** You cannot undo past the beginning of the game.  Please retry."
					<< endl;
			}
			else {
				undoLastMove(lastMoveMade, moveNumber);	// Undo last move in list
			}
			displayBoard(board, lastMoveMade);
			continue;   // Prompt to retry move
		}
		else if (userInput == 'l') {
			displayList(lastMoveMade);	// Display GameMove boards for debugging
			continue;
		}
		// Input is likely numeric and is not 'x' or 'u'.  Convert to a number.
		source = userInput - '0';
		cin >> userInput;	// Also get destination portion of user input
		destination = userInput - '0';
		if (source < 1 || source > 5) {	// Check if source is off board
			cout << "*** You can't refer to a position off the board. Invalid move, please retry."
				<< endl;
			continue;
		}

		if (destination < 1 || destination > 5) {	// Check if destination is off board
			cout << "*** You can't refer to a position off the board. Invalid move, please retry."
				<< endl;
			continue;
		}


		// Adjust user entry to compensate for 0-based indexing, rather than 1-based
		source--;
		destination--;

		// break out of enclosing loop, as we don't need to undo a move
		break;
	}
}//end promptForAndGetMove()


 // Check if source X can be moved
bool cantMoveX(char board[], int source, int destination) {
	if (board[source] == 'X') {
		if (destination < source) {	// Check if X is being moved left
			cout << "*** You can't move that piece that direction. ";
			return true;
		}
		if ((destination - source) > 2) {	// Check if X is being moved too far
			cout << "*** Destination is too far away. ";
			return true;
		}
		if (!(board[source + 1] == ' '	// Check if adjacent square is empty
			|| (board[source + 1] == 'O' && board[source + 2] == ' '))) {	// or is O and next to empty
			cout << "*** A jumped square must have an opponent. ";
			return true;
		}
	}
	return false;
}


// Check if source O can be moved
bool cantMoveO(char board[], int source, int destination) {
	if (board[source] == 'O') {
		if (destination > source) {	// Check if O is being moved left
			cout << "*** You can't move that piece that direction. ";
			return true;
		}
		if (fabs(destination - source) > 2) {	// Check if O is being moved too far
			cout << "*** Destination is too far away. ";
			return true;
		}
		if (!(board[source - 1] == ' '	// Check if adjacent square is empty
			|| (board[source - 1] == 'X' && board[source - 2] == ' '))) {	// or is X and next to empty
			cout << "*** A jumped square must have an opponent. ";
			return true;
		}
	}
	return false;
}


// Reed's code
//--------------------------------------------------------------------------------
bool moveNotValid(
	char board[],
	int source,
	int destination)
{
	// Move validation code should go here...
	if (board[destination] != ' ') {	// Check if destination is empty
		cout << "*** Destination square is not empty. ";
		return true;
	}
	if (cantMoveX(board, source, destination)) {	// Check if source X can be moved
		return true;
	}
	if (cantMoveO(board, source, destination)) {	// Check if source O can be moved
		return true;
	}
	return false;
}


// Reed's code is used
int main() {
	char board[BoardSize + 1] = "XX OO";  // extra character for the NULL
	int source, destination;
	int moveNumber = 1;
	GameMove *lastMoveMade = NULL;	// Game move for linked list of moves

	addGameMovetoList(board, moveNumber, lastMoveMade);	// Adds game move to linked list

	displayProgrammerInfo();	// Displays programmer's information
	displayIntroduction();		// Displays introduction


	displayBoard(board, lastMoveMade);

	// Game play loop
	while (notDone(board)) {
		promptForAndGetMove(board, moveNumber, source, destination, lastMoveMade);

		if (moveNotValid(board, source, destination)) {
			cout << "Invalid move, please retry. \n";
			continue;
		}

		// Make move.  
		board[destination] = board[source];
		board[source] = ' ';
		moveNumber++;
		addGameMovetoList(board, moveNumber, lastMoveMade);	// Adds game move to linked list

		displayBoard(board, lastMoveMade);
	}

	cout << "Congratulations, you did it! \n"
		<< "\n"
		<< "Exiting program ...\n";

	cin.get();
	cin.ignore();
	return 0;
}