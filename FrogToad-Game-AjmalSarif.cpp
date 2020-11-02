
// Coded by Ajmal Sarif | w1722688 - 2020 
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
using namespace std;

bool playerturn;

int numcheck(int num ,string message) {
	cout << message;
	while (!(cin >> num)) {

		cout << "\n----- Only Integers Allowed! -----" << endl << endl;
		cout << message;
		cin.clear();
		cin.ignore(124, '\n');
	}
	return num;
}

void boardcheck(int& board_length) {
	int board_valid;
	while (true) {
		board_valid = numcheck(board_length, "Enter Board Size: ");

		if (board_valid > 2 && board_valid < 31) {
			board_length = board_valid;
			break;
		}
		else {
			cout << "\nBoard Size needs to be 3 - 30!\n\n";
		}
	}


}

void tokencheck(int& token_size, int board_length) {
	int token_valid;
	int boardsize;
	while (true) {
		token_valid = numcheck(token_size, "Enter amount of Tokens : ");

		if ( board_length % 2 ==0) {
			boardsize = (board_length / 2) - 1;

		}
		else {
			boardsize = board_length / 2;
		}
		if (token_valid > 0 && token_valid <= boardsize ) {
			token_size = token_valid;
			break;
		}
		else {
			cout << "\nToken Size needs to be 1 - "<< boardsize <<"!\n";
		}
	}


}

void playercheck(int& p_input, vector <int>& board, int board_length) {
	
	int input_pos;
	int move_right;

	while (true) {
		// Check input, then move 1 position to right
		input_pos = numcheck(p_input, "Where to jump from? : ");
		move_right = input_pos--;

		// Disable moves from empty space and enemy
		if (board[input_pos] == 0 || board[input_pos] == 2) {
			cout << "\nInvalid Move!\n\n";
			continue;
		}

		// Check if move is not outside board
		if (move_right >= board_length ) {
			cout << "\nCan't go further!\n\n";
			continue;
		}

		//Move Player 1 space right
		if (board[move_right] == 0) {
			board[input_pos] = 0; // Clear old position
			board[move_right] = 1; // Draw new position
			break;
		}

		// Check if move is not outside board
		if (move_right+1 >= board_length) {
			cout << "\nCan't go further!\n\n";
			continue;
		}

		//Move Player 2 spaces right, 1 is frog, 2 is toad
		else if ((board[move_right] == 1 || board[move_right] == 2) && board[move_right + 1] == 0) {
			board[input_pos] = 0;
			board[move_right + 1] = 1;
			break;
		}
		else {
			cout << "\nPosition occuppied!\n\n";
		}

	}	
	playerturn = false;
}

void enemy_move(vector<int>& board, int board_length, int token_size) {

	vector <int> enemy_positions;
	int enemy_pos;
	int move_left;
	int randindex;
	
	// Get all index values of toads
	for (int x = 0; x < board_length;x++) {
		if (board[x] == 2){
			enemy_positions.push_back(x);
		}
	}

	while (true) {
		

		//Create Random Index number from token size
		int randindex = rand() % token_size;
		enemy_pos = enemy_positions[randindex];
		move_left = enemy_pos-1;
		
		

		//Moves outside board not allowed
		if (move_left < 0 ) {
			
			continue; // To takes a new random position
			
		}

		//Move Enemy 1 space  left
		if (board[move_left] == 0) {
			board[enemy_pos] = 0; // Clear old position
			board[move_left] = 2; // Draw new position
			break;
		} 

		//Moves outside board not allowed
		if (move_left+1 < 0) {
			
			continue; // To takes a new random position

		}
		//Move Enemy 2 spaces left
		else if ((board[move_left] == 1 || board[move_left] == 2) && board[move_left - 1] == 0) {
			board[enemy_pos] = 0;
			board[move_left - 1] = 2;
			break;
		}
	}
	playerturn = true;

	
}

void drawboard(vector <int>& board, int board_length, bool fog_status) {
	int fogstart;
	int fogend;
	bool fogstart_found = false;

#pragma region Border & Numbers
	int spaces;
	//Display Number Label
	cout << endl << endl << "\t  ";
	for (int i = 1; i < board_length + 1; i++) {
		(i > 9) ? spaces = 2 : spaces = 3;
		cout << i << string(spaces, ' ');
	}
	cout << endl << "\t";
	for (int i = 1; i < (board_length * 4) + 2; i++) {
		cout << "-";
	}
#pragma endregion

	if (fog_status==true) {
		// Thick Fog 
		for (int i = 0; i < board_length; i++) {
			//Find start 
			if (fogstart_found == false && board[i] == 1) {
				fogstart = i;
				fogend = i;
				fogstart_found = true;
			}
			//Find End
			else if (board[i] == 1) {
				fogend = i; // Each time Frog is found update, so last frog will be last updated position of frog
			}

		}
	}
	
	

	// Display Main Board
	cout << endl << "\t";
	for (int i = 0; i < board_length; i++) {
		if (fog_status == true) {
			// Creates the fog if i is less than 2 squares away from frogs
			if (i < fogstart - 2 || i > fogend + 2) {
				cout << "| " << "#" << " ";
				continue;
			}
		}
		if (board[i] == 0) {
			cout << "| " << " " << " ";
		}
		else if (board[i] == 1) {
			cout << "| " << "F" << " ";
		}
		else if (board[i] == 2) {
			cout << "| " << "T" << " ";
		}

	}


#pragma region Border
	cout << "|" << endl << "\t";
	//Display Borders
	for (int i = 1; i < (board_length * 4) + 2; i++) {
		cout << "-";
	}
	cout << endl << endl;

#pragma endregion  


	
}

void movecheck(vector<int>& board, int board_length) {

	int available_frog_moves = 0;
	int available_toad_moves = 0;
	int move_right;
	int move_left;
	for (int i = 0; i < board_length; i++) {

		//Frog Moves Check
		if (board[i] == 1) {
			move_right = i + 1;

			//Check move is not outside board
			if (move_right >= board_length) {
				continue;
			}
			//Check for moves to right, 1 step
			if (board[move_right] == 0) {

				available_frog_moves++;
			}
			//Check move is not outside board
			if (move_right + 1 >= board_length) {
				continue;
			}
			//Check for moves to right, 2 steps
			else if ((board[move_right] == 1 || board[move_right] == 2) && board[move_right + 1] == 0) {

				available_frog_moves++;
			}
		}
		//Toad Moves Check
		else if (board[i] == 2) {

			move_left = i - 1;
			//Moves outside board not allowed
			if (move_left < 0) {
				continue;
			}
			//Move Enemy 1 space  left
			if (board[move_left] == 0) {

				available_toad_moves++;

			}
			//Moves outside board not allowed
			if (move_left - 1 < 0) {
				continue; // To take next move
			}
			//Move Enemy 2 spaces left
			else if ((board[move_left] == 1 || board[move_left] == 2) && board[move_left - 1] == 0) {

				available_toad_moves++;

			}
		}

	}
	

	if (available_toad_moves == 0 && playerturn == false) {
		system("CLS");
		drawboard(board, board_length,false);
		cout << " Toad has no moves" << endl;
		cout << "---> FROG WON! <---\n";
		exit(0);
	}
	else if (available_frog_moves == 0 && playerturn == true ) {
		system("CLS");
		drawboard(board, board_length,false);
		cout << " Frog has no moves" << endl;
		cout << "===> TOAD WON! <===\n";
		exit(0);
	}
}

void toadwait(vector<int>& board, int board_length) {

	//Random int from 2-5
	int randint = rand() % 5 + 2; 
	//Update Board 
	system("CLS");
	drawboard(board, board_length, false);
	cout << "Toad Turn";
	Sleep(randint*100);
	//Draw thinking dots
	for (int u = 0; u < 3; u++) {
		cout << ".";
		Sleep(randint * 100);
	}
	cout << endl;

}

int main()
{

	int board_length;
	int token_size;
	int token_left;
	int token_right;
	int player_input;
	bool player_turn;

	//Check if Inputs are correct
	boardcheck(board_length);
	tokencheck(token_size,board_length);
	token_left = token_size;
	token_right = board_length - token_size;

	vector <int> board(board_length,0);
	
#pragma region Initialize	
	//Initialize Random Seed
	srand(time(NULL));

	//Create starting tokens
	for (int l = 0; l < token_left; l++) {
		board[l] = 1;
	}
	for (int r = token_right;  r < board_length ; r++) {
		board[r] = 2;
	}
	system("CLS");
	cout << "Instructions\nType number of field you want frog to move!\n\nHow to win?\nIf anyone has no moves for next round they loose!\n\n" << endl;
	cin.ignore();
	cout << "Press Any key to continue...";
	cin.get();
	system("CLS");
#pragma endregion


#pragma region Update

	while (true) {
		
		//Update Board 
		drawboard(board, board_length,true);
		
		cout << "Your Turn" << endl;
		//Player Move (Frog) 
		playercheck(player_input, board, board_length);
		
		//Moves Check 
		movecheck(board, board_length);

		//Update Board
		toadwait(board,board_length);

		//AI Move (Toad)
		enemy_move(board, board_length, token_size);

		//Moves Check 
		movecheck(board, board_length);

		//Clear Console
		system("CLS");

		
	}

#pragma endregion

}

